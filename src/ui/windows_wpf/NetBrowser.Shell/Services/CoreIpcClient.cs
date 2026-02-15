using System;
using System.Net.WebSockets;
using System.Text;
using System.Threading;
using System.Threading.Tasks;

namespace NetBrowser.Shell.Services;

public sealed class CoreIpcClient
{
  private ClientWebSocket? _ws;
  private int _id = 1;

  public event Action<string>? OnLine;

  public int NextId() => Interlocked.Increment(ref _id);

  public async void ConnectAsync()
  {
    try
    {
      _ws = new ClientWebSocket();
      await _ws.ConnectAsync(new Uri("ws://127.0.0.1:8787/ws"), CancellationToken.None);
      OnLine?.Invoke("[wpf] connected");
      _ = Task.Run(ReadLoop);
    }
    catch (Exception ex)
    {
      OnLine?.Invoke("[wpf] connect error: " + ex.Message);
    }
  }

  public async void Send(string text)
  {
    try
    {
      if (_ws == null || _ws.State != WebSocketState.Open) return;
      var data = Encoding.UTF8.GetBytes(text);
      await _ws.SendAsync(data, WebSocketMessageType.Text, true, CancellationToken.None);
      OnLine?.Invoke("=> " + text);
    }
    catch (Exception ex)
    {
      OnLine?.Invoke("[wpf] send error: " + ex.Message);
    }
  }

  private async Task ReadLoop()
  {
    if (_ws == null) return;
    var buf = new byte[64 * 1024];
    while (_ws.State == WebSocketState.Open)
    {
      var r = await _ws.ReceiveAsync(buf, CancellationToken.None);
      if (r.MessageType == WebSocketMessageType.Close) break;
      var txt = Encoding.UTF8.GetString(buf, 0, r.Count);
      OnLine?.Invoke("<= " + txt);
    }
  }
}