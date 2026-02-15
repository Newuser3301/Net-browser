# Net-browser 🚀🧱
### A Polyglot, IPC‑First Browser Architecture Demo

**Net-browser** is not just another browser.
This project is a **fully working marketing + engineering demo** that shows **how a modern browser can be built**.

> If you are looking for a **Chrome clone** — this is not a repo.
> If you want to see **real system design, IPC, multi‑process and multi‑language architecture** — you are welcome 😎

---

## 🔥 Why Net-browser?
Today's browsers:
- very large monolithic code bases
- difficult to learn
- does not answer the question "what works where"

**Net-browser** takes a different approach:

- 🧱 **Process-first design** — core, bridge, UI separated
- 🔌 **Explicit IPC contract** — who talks to whom and how is clear
- 🧩 **Polyglot stack** — each language has its strengths
- 🎯 **Marketing-friendly demo** — ideal for GitHub, portfolio, presentation

---

## 🧠 Architecture at a Glance

```
WebUI (HTML/JS) ──┐
│ WebSocket
WPF GUI (C#) ─┤
▼
Bridge (.NET 8, C#)
│ Named Pipe
▼
Core Process (C/C++)
```

- **Core** — “brain”: tab, navigation, event lifecycle
- **Bridge** — translator: OS‑level IPC ↔ UI‑friendly WebSocket
- ​​**UI** — client: Web or Desktop

This design:
- makes testing easier
- Makes UI switching easier
- ready to add sandbox / isolation in the future

---

## 🧬 Tech Stack (10+ languages, real use)

| Layer | Technology Why?
|-----|-----------|-----|
| Core | C / C++ | Performance, low-level IPC |
| Bridge | C# (.NET 8) | Fast dev, Windows native |
| WebUI | HTML / CSS / JS | Zero install, demo-ready
| Desktop UI | WPF (C#) | Native GUI |
| Helpers | Rust | Safe parsing example |
| Platform | C (ABI) | OS abstraction |
| Contracts | Proto / YAML | Clear boundaries
| GPU | GLSL | Graphics pipeline stub |
| Tools | Python / Lua / Perl / Tcl | Automation & legacy vibes |

👉 **This is not an “artificial multi-language” — each one performs a real task.**

---

## 🎮 What You Can Demo (Live)
After launching:

- **PING** in WebUI → Real response from Core
- **New Tab** → Core state change
- **Navigate** → events return in real-time
- Connect WPF GUI and control the same core

All of this:
- Separate process
- Real IPC
- No fake mocks

---

## 🚀 Quick Start (Windows)

### 1️⃣ Dependencies
- Windows 10 / 11
- Visual Studio 2022 Build Tools (C++)
- CMake
- PowerShell 7+
- .NET 8 SDK

**Recommended method (Chocolatey):**
```powershell
choco install cmake -y
choco install dotnet-8.0-sdk -y
refreshenv
```

---

### 2️⃣ Build Core
``powershell
.\scripts\build.ps1
```

Result:
```
build\Release\netbrowser_core.exe
```

---

### 3️⃣ Run (3 terminals)
**Terminal 1 — Core**
``powershell
.\scripts\run_core.ps1
```

**Terminal 2 — Bridge**
``powershell
.\scripts\run_bridge.ps1
```

**Terminal 3 — WebUI**
``powershell
.\scripts\run_webui.ps1
```

Open in browser:
```
http://127.0.0.1:5173
```

---

## 🪟 Desktop GUI (Optional)
``powershell
dotnet run --project src/ui/windows_wpf/NetBrowser.Shell/NetBrowser.Shell.csproj
```
Must be Bridge running.

---

## 💡 Who Is This For?
- 🧑‍💻 System / backend / infra developers
- 🧠 Architecture learners
- 📂 Portfolio / GitHub showcase
- 🎤 Tech talk / demo to do
- 🧪 Browser inside out enthusiasts

---

## 🛣️ Roadmap (Vision)
- Binary / Protobuf IPC
- Process isolation & sandbox
- Embedded renderer (WebView2)
- Privacy-first feature flags
- Crash recovery & supervision

This repo is a **starting point**.

---

## ⚠️ Honest Disclaimer
Net-browser:
- not a production browser
- not security-hardened
- not a full rendering engine

This is an **architecture + integration demo**.
It was chosen this way on purpose.

---

## 📜 License
MIT

---

### ⭐ If this repo helped you
- Star ⭐
- Fork
- Build your own demo
