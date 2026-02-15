#include "src/ipc/include/ipc.h"

#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <string>
#include <vector>

namespace netbrowser::ipc {

static bool ReadExact(HANDLE h, void* buf, DWORD n, IpcError* err) {
  BYTE* p = static_cast<BYTE*>(buf);
  DWORD got = 0;
  while (got < n) {
    DWORD r = 0;
    BOOL ok = ReadFile(h, p + got, n - got, &r, nullptr);
    if (!ok || r == 0) {
      if (err) {
        err->code = (int)GetLastError();
        err->message = "ReadFile failed or disconnected";
      }
      return false;
    }
    got += r;
  }
  return true;
}

static bool WriteAll(HANDLE h, const void* buf, DWORD n, IpcError* err) {
  const BYTE* p = static_cast<const BYTE*>(buf);
  DWORD sent = 0;
  while (sent < n) {
    DWORD w = 0;
    BOOL ok = WriteFile(h, p + sent, n - sent, &w, nullptr);
    if (!ok) {
      if (err) {
        err->code = (int)GetLastError();
        err->message = "WriteFile failed";
      }
      return false;
    }
    sent += w;
  }
  FlushFileBuffers(h);
  return true;
}

NamedPipeServer::NamedPipeServer() : hPipe_(nullptr), connected_(false) {}
NamedPipeServer::~NamedPipeServer() { Close(); }

bool NamedPipeServer::Start(std::wstring pipePath, IpcError* err) {
  pipePath_ = std::move(pipePath);

  HANDLE h = CreateNamedPipeW(
    pipePath_.c_str(),
    PIPE_ACCESS_DUPLEX,
    PIPE_TYPE_BYTE | PIPE_READMODE_BYTE | PIPE_WAIT,
    1,
    64 * 1024,
    64 * 1024,
    0,
    nullptr
  );

  if (h == INVALID_HANDLE_VALUE) {
    if (err) {
      err->code = (int)GetLastError();
      err->message = "CreateNamedPipeW failed";
    }
    return false;
  }

  hPipe_ = h;
  connected_ = false;
  return true;
}

bool NamedPipeServer::AcceptOneClient(IpcError* err) {
  if (!hPipe_) {
    if (err) { err->code = -1; err->message = "pipe not started"; }
    return false;
  }

  BOOL ok = ConnectNamedPipe((HANDLE)hPipe_, nullptr);
  if (!ok) {
    DWORD e = GetLastError();
    if (e != ERROR_PIPE_CONNECTED) {
      if (err) {
        err->code = (int)e;
        err->message = "ConnectNamedPipe failed";
      }
      return false;
    }
  }

  connected_ = true;
  return true;
}

bool NamedPipeServer::ReadFrame(Frame* out, IpcError* err) {
  if (!connected_) {
    if (err) { err->code = -2; err->message = "not connected"; }
    return false;
  }

  uint32_t len = 0;
  if (!ReadExact((HANDLE)hPipe_, &len, sizeof(len), err)) {
    connected_ = false;
    return false;
  }

  if (len == 0 || len > kMaxFrameBytes) {
    if (err) { err->code = -3; err->message = "bad frame length"; }
    connected_ = false;
    return false;
  }

  std::vector<char> buf(len);
  if (!ReadExact((HANDLE)hPipe_, buf.data(), (DWORD)len, err)) {
    connected_ = false;
    return false;
  }

  out->payload.assign(buf.begin(), buf.end());
  return true;
}

bool NamedPipeServer::WriteFrame(const Frame& f, IpcError* err) {
  if (!connected_) {
    if (err) { err->code = -2; err->message = "not connected"; }
    return false;
  }
  uint32_t len = (uint32_t)f.payload.size();
  if (len == 0 || len > kMaxFrameBytes) {
    if (err) { err->code = -3; err->message = "bad frame length"; }
    return false;
  }
  if (!WriteAll((HANDLE)hPipe_, &len, sizeof(len), err)) return false;
  return WriteAll((HANDLE)hPipe_, f.payload.data(), len, err);
}

void NamedPipeServer::Close() {
  if (hPipe_) {
    HANDLE h = (HANDLE)hPipe_;
    hPipe_ = nullptr;
    if (connected_) {
      FlushFileBuffers(h);
      DisconnectNamedPipe(h);
    }
    CloseHandle(h);
  }
  connected_ = false;
}

} // namespace netbrowser::ipc