#include "src/core/browser/app/browser_process.h"

namespace netbrowser::core {

BrowserProcess::BrowserProcess() : nextTabId_(1) {
  // Start with one blank tab by default
  NewTab("about:blank");
}

int32_t BrowserProcess::NewTab(const std::string& url) {
  TabState t;
  t.tab_id = nextTabId_++;
  t.url = url;
  t.title = (url == "about:blank") ? "New Tab" : "Loading...";
  t.load = LoadState::Idle;
  tabs_[t.tab_id] = t;
  return t.tab_id;
}

bool BrowserProcess::CloseTab(int32_t tabId) {
  return tabs_.erase(tabId) > 0;
}

TabState* BrowserProcess::GetTab(int32_t tabId) {
  auto it = tabs_.find(tabId);
  if (it == tabs_.end()) return nullptr;
  return &it->second;
}

bool BrowserProcess::Navigate(int32_t tabId, const std::string& url) {
  auto* t = GetTab(tabId);
  if (!t) return false;
  t->url = url;
  t->title = "Loading...";
  t->load = LoadState::Loading;
  return true;
}

bool BrowserProcess::Back(int32_t tabId) {
  auto* t = GetTab(tabId);
  if (!t) return false;
  // MVP: no history yet
  return true;
}

bool BrowserProcess::Forward(int32_t tabId) {
  auto* t = GetTab(tabId);
  if (!t) return false;
  // MVP: no history yet
  return true;
}

bool BrowserProcess::Reload(int32_t tabId) {
  auto* t = GetTab(tabId);
  if (!t) return false;
  t->load = LoadState::Loading;
  return true;
}

} // namespace netbrowser::core