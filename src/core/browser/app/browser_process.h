#pragma once
#include <cstdint>
#include <string>
#include <unordered_map>

namespace netbrowser::core {

enum class LoadState : uint8_t { Idle = 0, Loading = 1, Error = 2 };

struct TabState {
  int32_t tab_id = 1;
  std::string url;
  std::string title;
  LoadState load = LoadState::Idle;
};

class BrowserProcess {
public:
  BrowserProcess();

  int32_t NewTab(const std::string& url);
  bool CloseTab(int32_t tabId);

  bool Navigate(int32_t tabId, const std::string& url);
  bool Back(int32_t tabId);
  bool Forward(int32_t tabId);
  bool Reload(int32_t tabId);

  TabState* GetTab(int32_t tabId);
  const std::unordered_map<int32_t, TabState>& Tabs() const { return tabs_; }

private:
  int32_t nextTabId_;
  std::unordered_map<int32_t, TabState> tabs_;
};

} // namespace netbrowser::core