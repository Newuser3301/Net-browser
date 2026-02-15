#pragma once
#include <cstdint>
#include <string>
#include <vector>

namespace netbrowser::ipc {

static constexpr const wchar_t* kPipePath = L"\\\\.\\pipe\\netbrowser.ipc.v1";
static constexpr uint32_t kMaxFrameBytes = 1024u * 1024u;

struct IpcError {
  int code = 0;
  std::string message;
};

struct Frame {
  std::string payload; // UTF-8 text payload
};

class NamedPipeServer {
public:
  NamedPipeServer();
  ~NamedPipeServer();

  bool Start(std::wstring pipePath, IpcError* err);
  bool AcceptOneClient(IpcError* err);

  // Blocking read (returns false if disconnected)
  bool ReadFrame(Frame* out, IpcError* err);

  // Send one frame
  bool WriteFrame(const Frame& f, IpcError* err);

  void Close();

private:
  void* hPipe_; // HANDLE but avoid windows.h here
  std::wstring pipePath_;
  bool connected_;
};

} // namespace netbrowser::ipc