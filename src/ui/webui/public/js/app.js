(() => {
  const $ = (s) => document.querySelector(s);

  const logEl = $("#log");
  const statusEl = $("#status");
  const urlEl = $("#url");
  const goBtn = $("#go");
  const newTabBtn = $("#newTab");
  const pingBtn = $("#ping");

  const state = { ws: null, req: 1 };

  function log(msg) {
    const line = `[webui] ${msg}`;
    console.log(line);
    if (logEl) {
      logEl.textContent += line + "\n";
      logEl.scrollTop = logEl.scrollHeight;
    }
  }

  function setStatus(s) {
    if (!statusEl) return;
    statusEl.textContent = s;
    statusEl.style.color = (s === "connected") ? "#9be29b" : "#d6a1a1";
  }

  function nextId() { return state.req++; }

  function connect() {
    const ws = new WebSocket("ws://127.0.0.1:8787/ws");
    state.ws = ws;

    ws.onopen = () => { setStatus("connected"); log("ws connected"); };
    ws.onclose = () => { setStatus("disconnected"); log("ws closed"); };
    ws.onerror = () => { setStatus("error"); log("ws error"); };

    ws.onmessage = (ev) => {
      const txt = String(ev.data || "");
      log("<= " + txt);
    };
  }

  function send(cmd) {
    if (!state.ws || state.ws.readyState !== 1) {
      log("ws not ready. start bridge first.");
      return;
    }
    log("=> " + cmd);
    state.ws.send(cmd);
  }

  if (goBtn && urlEl) {
    goBtn.addEventListener("click", () => {
      const url = urlEl.value.trim();
      if (!url) return;
      send(`NAV|${nextId()}|${url}`);
    });
  }

  if (newTabBtn) {
    newTabBtn.addEventListener("click", () => {
      send(`NEWTAB|${nextId()}`);
    });
  }

  if (pingBtn) {
    pingBtn.addEventListener("click", () => {
      send(`PING|${nextId()}`);
    });
  }

  connect();
})();