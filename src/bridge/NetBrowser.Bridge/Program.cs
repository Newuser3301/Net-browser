using System;
using System.Collections.Concurrent;
using System.IO;
using System.IO.Pipes;
using System.Net;
using System.Net.WebSockets;
using System.Text;
using System.Threading;
using System.Threading.Tasks;

static class Framing {
  public static byte[] Pack(string payload) {
    var body = Encoding.UTF8.GetBytes(payload);
    if (body.Length <= 0 || body.Length > 1024 * 1024) throw new Exception("payload too large");
    var buf = new byte[4 + body.Length];
    int n = body.Length;
    buf[0] = (byte)(n & 0xFF);
    buf[1] = (byte)((n >> 8) & 0xFF);
    buf[2] = (byte)((n >> 16) & 0xFF);
    buf[3] = (byte)((n >> 24) & 0xFF);
    Buffer.BlockCopy(body, 0, buf, 4, body.Length);
    return buf;
  }

  public static async Task<string?> ReadFrameAsync(Stream s, CancellationToken ct) {
    var lenBuf = new byte[4];
    if (!await ReadExactAsync(s, lenBuf, 4, ct)) return null;
    int n = lenBuf[0] | (lenBuf[1] << 8) | (lenBuf[2] << 16) | (lenBuf[3] << 24);
    if (n <= 0 || n > 1024 * 1024) throw new Exception("bad frame length");
    var payload = new byte[n];
    if (!await ReadExactAsync(s, payload, n, ct)) return null;
    return Encoding.UTF8.GetString(payload);
  }

  private static async Task<bool> ReadExactAsync(Stream s, byte[] buf, int n, CancellationToken ct) {
    int got = 0;
    while (got < n) {
      int r = await s.ReadAsync(buf.AsMemory(got, n - got), ct);
      if (r <= 0) return false;
      got += r;
    }
    return true;
  }
}

sealed class PipeLink : IAsyncDisposable {
  private readonly NamedPipeClientStream _pipe;
  private readonly CancellationTokenSource _cts = new();
  private Task? _reader;

  public event Action<string>? OnMessage;

  public PipeLink(string pipeName) {
    _pipe = new NamedPipeClientStream(".", pipeName, PipeDirection.InOut, PipeOptions.Asynchronous);
  }

  public async Task ConnectAsync(int timeoutMs) {
    await _pipe.ConnectAsync(timeoutMs);
    _reader = Task.Run(async () => {
      try {
        while (!_cts.IsCancellationRequested) {
          var msg = await Framing.ReadFrameAsync(_pipe, _cts.Token);
          if (msg == null) break;
          OnMessage?.Invoke(msg);
        }
      } catch (Exception ex) {
        Console.Error.WriteLine($"[bridge] pipe read: {ex.Message}");
      }
    });
  }

  public async Task SendAsync(string payload, CancellationToken ct) {
    var buf = Framing.Pack(payload);
    await _pipe.WriteAsync(buf, 0, buf.Length, ct);
    await _pipe.FlushAsync(ct);
  }

  public bool IsConnected => _pipe.IsConnected;

  public async ValueTask DisposeAsync() {
    try { _cts.Cancel(); } catch {}
    try { if (_reader != null) await _reader; } catch {}
    _pipe.Dispose();
    _cts.Dispose();
  }
}

sealed class WsHub {
  private readonly ConcurrentDictionary<Guid, WebSocket> _clients = new();

  public void Add(Guid id, WebSocket ws) => _clients[id] = ws;
  public void Remove(Guid id) => _clients.TryRemove(id, out _);

  public async Task BroadcastAsync(string text, CancellationToken ct) {
    var data = Encoding.UTF8.GetBytes(text);
    var seg = new ArraySegment<byte>(data);
    foreach (var kv in _clients) {
      var ws = kv.Value;
      if (ws.State != WebSocketState.Open) continue;
      try { await ws.SendAsync(seg, WebSocketMessageType.Text, true, ct); } catch {}
    }
  }
}

static async Task MainLoop() {
  const string PipeName = "netbrowser.ipc.v1"; // without \\.\pipe\
  const string Prefix = "http://127.0.0.1:8787/";

  var hub = new WsHub();

  using var http = new HttpListener();
  http.Prefixes.Add(Prefix);
  http.Start();
  Console.WriteLine("[bridge] ws listening: ws://127.0.0.1:8787/ws");

  await using var pipe = new PipeLink(PipeName);
  pipe.OnMessage += (m) => _ = hub.BroadcastAsync(m, CancellationToken.None);

  try {
    await pipe.ConnectAsync(4000);
    Console.WriteLine("[bridge] connected to core pipe");
    await pipe.SendAsync("HELLO|WebUI|0.1.0", CancellationToken.None);
  } catch (Exception ex) {
    Console.Error.WriteLine($"[bridge] cannot connect to core pipe. start core first. {ex.Message}");
  }

  while (true) {
    var ctx = await http.GetContextAsync();
    if (!ctx.Request.IsWebSocketRequest || ctx.Request.Url == null || ctx.Request.Url.AbsolutePath != "/ws") {
      ctx.Response.StatusCode = 404;
      ctx.Response.Close();
      continue;
    }

    var wsCtx = await ctx.AcceptWebSocketAsync(null);
    var ws = wsCtx.WebSocket;
    var id = Guid.NewGuid();
    hub.Add(id, ws);
    Console.WriteLine("[bridge] web client connected");

    _ = Task.Run(async () => {
      var buf = new byte[64 * 1024];
      try {
        while (ws.State == WebSocketState.Open) {
          var r = await ws.ReceiveAsync(buf, CancellationToken.None);
          if (r.MessageType == WebSocketMessageType.Close) break;

          var txt = Encoding.UTF8.GetString(buf, 0, r.Count).Trim();
          if (txt.Length == 0) continue;

          if (pipe.IsConnected) {
            await pipe.SendAsync(txt, CancellationToken.None);
          } else {
            var err = Encoding.UTF8.GetBytes("EVT|BridgeError|0|{\"msg\":\"core_not_connected\"}");
            await ws.SendAsync(err, WebSocketMessageType.Text, true, CancellationToken.None);
          }
        }
      } catch (Exception ex) {
        Console.Error.WriteLine($"[bridge] ws loop: {ex.Message}");
      } finally {
        try { await ws.CloseAsync(WebSocketCloseStatus.NormalClosure, "bye", CancellationToken.None); } catch {}
        hub.Remove(id);
        Console.WriteLine("[bridge] web client disconnected");
      }
    });
  }
}

await MainLoop();