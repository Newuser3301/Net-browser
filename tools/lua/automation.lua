-- Net-browser automation stub
-- Purpose: demonstrate Lua being used for small automation glue (not wired).
local M = {}

function M.smoke()
  return {
    name = "net-browser-smoke",
    steps = {
      "start_core",
      "start_bridge",
      "open_webui",
      "send_ping",
      "send_newtab",
      "send_nav"
    }
  }
end

return M