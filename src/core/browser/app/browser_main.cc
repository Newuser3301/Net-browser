#include "src/ipc/include/ipc.h"
#include "src/core/browser/app/browser_process.h"
#include "src/platform/c_shim/include/net_platform.h"

#include <iostream>
#include <sstream>
#include <string>
#include <vector>

using netbrowser::ipc::Frame;
using netbrowser::ipc::IpcError;
using netbrowser::ipc::NamedPipeServer;

static std::vector<std::string> Split(const std::string& s, char delim) {
  std::vector<std::string> out;
  std::string cur;
  for (char c : s) {
    if (c == delim) { out.push_back(cur); cur.clear(); }
    else { cur.push_back(c); }
  }
  out.push_back(cur);
  return out;
}

static std::string JsonKV(const std::string& k, const std::string& v) {
  std::string esc;
  esc.reserve(v.size()+8);
  for (char c : v) {
    if (c == '\\' || c == '"') { esc.push_back('\\'); esc.push_back(c); }
    else if (c == '\n') { esc += "\\n"; }
    else if (c == '\r') { esc += "\\r"; }
    else if (c == '\t') { esc += "\\t"; }
    else { esc.push_back(c); }
  }
  return std::string("{\"") + k + "\":\"" + esc + "\"}";
}

static bool Send(NamedPipeServer& srv, const std::string& payload) {
  IpcError e;
  Frame f{ payload };
  if (!srv.WriteFrame(f, &e)) {
    std::cerr << "[core] send error: " << e.code << " " << e.message << "\n";
    return false;
  }
  return true;
}

int main() {
  std::cout << "[core] Net-browser core starting...\n";

  NamedPipeServer srv;
  IpcError err;
  if (!srv.Start(netbrowser::ipc::kPipePath, &err)) {
    std::cerr << "[core] pipe start failed: " << err.code << " " << err.message << "\n";
    return 1;
  }

  std::wcout << L"[core] listening on " << netbrowser::ipc::kPipePath << L" .....\n";
  if (!srv.AcceptOneClient(&err)) {
    std::cerr << "[core] accept failed: " << err.code << " " << err.message << "\n";
    return 2;
  }

  std::cout << "[core] client connected\n";
  Send(srv, "HELLO|NetBrowser.Core|0.1.0|1|0");

  netbrowser::core::BrowserProcess browser;

  while (true) {
    Frame in;
    if (!srv.ReadFrame(&in, &err)) {
      std::cerr << "[core] client disconnected (" << err.code << " " << err.message << ")\n";
      break;
    }

    auto parts = Split(in.payload, '|');
    if (parts.empty()) continue;

    const std::string& cmd = parts[0];

    if (cmd == "HELLO") {
      // HELLO|name|version
      Send(srv, "OK|0|1|hello_ack");
      continue;
    }

    if (cmd == "PING" && parts.size() >= 2) {
      Send(srv, "OK|" + parts[1] + "|1|pong");
      continue;
    }

    if (cmd == "NEWTAB" && parts.size() >= 2) {
      int tabId = browser.NewTab("about:blank");
      Send(srv, "OK|" + parts[1] + "|" + std::to_string(tabId) + "|newtab_created");
      Send(srv, "EVT|TitleChanged|" + std::to_string(tabId) + "|" + JsonKV("title", "New Tab"));
      Send(srv, "EVT|UrlChanged|" + std::to_string(tabId) + "|" + JsonKV("url", "about:blank"));
      Send(srv, "EVT|LoadStateChanged|" + std::to_string(tabId) + "|" + JsonKV("state", "idle"));
      continue;
    }

    if (cmd == "NAV" && parts.size() >= 3) {
      const std::string reqId = parts[1];
      const std::string url = parts[2];

      // For MVP we navigate active tab = 1 (or create if missing)
      int32_t tabId = 1;
      auto* t = browser.GetTab(tabId);
      if (!t) tabId = browser.NewTab("about:blank");

      browser.Navigate(tabId, url);
      Send(srv, "OK|" + reqId + "|" + std::to_string(tabId) + "|navigate_accepted");
      Send(srv, "EVT|UrlChanged|" + std::to_string(tabId) + "|" + JsonKV("url", url));
      Send(srv, "EVT|LoadStateChanged|" + std::to_string(tabId) + "|" + JsonKV("state", "loading"));

      // Fake load delay
      nb_sleep_ms(250);

      auto* tt = browser.GetTab(tabId);
      if (tt) {
        tt->title = "Loaded: " + url;
        tt->load = netbrowser::core::LoadState::Idle;
      }
      Send(srv, "EVT|TitleChanged|" + std::to_string(tabId) + "|" + JsonKV("title", "Loaded: " + url));
      Send(srv, "EVT|LoadStateChanged|" + std::to_string(tabId) + "|" + JsonKV("state", "idle"));
      continue;
    }

    // Unknown
    Send(srv, "OK|0|1|unknown_command");
  }

  srv.Close();
  std::cout << "[core] shutdown\n";
  return 0;
}