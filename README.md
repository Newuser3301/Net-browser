# Net-browser 🚀🧱
### A Polyglot, IPC‑First Browser Architecture Demo

**Net-browser** — bu oddiy “yana bitta brauzer” emas.  
Bu loyiha **zamonaviy brauzer qanday arxitektura bilan qurilishi mumkinligini** ko‘rsatadigan **to‘liq ishlaydigan marketing + engineering demo**.

> Agar siz **Chrome kloni** izlayotgan bo‘lsangiz — bu repo emas.  
> Agar siz **real system design, IPC, multi‑process va multi‑language arxitektura** ko‘rmoqchi bo‘lsangiz — xush kelibsiz 😎

---

## 🔥 Why Net-browser?
Bugungi brauzerlar:
- juda katta monolit kod bazalar
- o‘rganish qiyin
- “nima qayerda ishlayapti” degan savolga javob bermaydi

**Net-browser** esa boshqacha yo‘l tutadi:

- 🧱 **Process-first design** — core, bridge, UI ajratilgan
- 🔌 **Explicit IPC contract** — kim kim bilan qanday gaplashadi ochiq
- 🧩 **Polyglot stack** — har bir til o‘z kuchli joyida
- 🎯 **Marketing-friendly demo** — GitHub, portfolio, presentation uchun ideal

---

## 🧠 Architecture at a Glance

```
WebUI (HTML/JS)  ──┐
                   │ WebSocket
WPF GUI (C#)    ──┤
                   ▼
            Bridge (.NET 8, C#)
                   │ Named Pipe
                   ▼
          Core Process (C/C++)
```

- **Core** — “brain”: tab, navigation, event lifecycle
- **Bridge** — translator: OS‑level IPC ↔ UI‑friendly WebSocket
- **UI** — client: Web yoki Desktop

Bu dizayn:
- test qilishni osonlashtiradi
- UI’ni almashtirishni osonlashtiradi
- kelajakda sandbox / isolation qo‘shishga tayyor

---

## 🧬 Tech Stack (10+ languages, real use)

| Layer | Technology | Why |
|-----|-----------|-----|
| Core | C / C++ | Performance, low-level IPC |
| Bridge | C# (.NET 8) | Fast dev, Windows native |
| WebUI | HTML / CSS / JS | Zero install, demo-ready |
| Desktop UI | WPF (C#) | Native GUI |
| Helpers | Rust | Safe parsing example |
| Platform | C (ABI) | OS abstraction |
| Contracts | Proto / YAML | Clear boundaries |
| GPU | GLSL | Graphics pipeline stub |
| Tools | Python / Lua / Perl / Tcl | Automation & legacy vibes |

👉 **Bu “sun’iy ko‘p til” emas — har biri real vazifa bajaradi.**

---

## 🎮 What You Can Demo (Live)
Ishga tushirgandan keyin:

- WebUI’da **PING** → Core’dan real javob
- **New Tab** → Core state change
- **Navigate** → eventlar real-time qaytadi
- WPF GUI’ni ulab, xuddi shu core’ni boshqarish

Bularning barchasi:
- alohida process
- real IPC
- fake mock’siz

---

## 🚀 Quick Start (Windows)

### 1️⃣ Dependencies
- Windows 10 / 11
- Visual Studio 2022 Build Tools (C++)
- CMake
- PowerShell 7+
- .NET 8 SDK

**Tavsiya etilgan usul (Chocolatey):**
```powershell
choco install cmake -y
choco install dotnet-8.0-sdk -y
refreshenv
```

---

### 2️⃣ Build Core
```powershell
.\scripts\build.ps1
```

Natija:
```
build\Release\netbrowser_core.exe
```

---

### 3️⃣ Run (3 terminals)
**Terminal 1 — Core**
```powershell
.\scripts\run_core.ps1
```

**Terminal 2 — Bridge**
```powershell
.\scripts\run_bridge.ps1
```

**Terminal 3 — WebUI**
```powershell
.\scripts\run_webui.ps1
```

Browser’da oching:
```
http://127.0.0.1:5173
```

---

## 🪟 Desktop GUI (Optional)
```powershell
dotnet run --project src/ui/windows_wpf/NetBrowser.Shell/NetBrowser.Shell.csproj
```
Bridge running bo‘lishi shart.

---

## 💡 Who Is This For?
- 🧑‍💻 System / backend / infra developerlar
- 🧠 Architecture o‘rganayotganlar
- 📂 Portfolio / GitHub showcase
- 🎤 Tech talk / demo qilish uchun
- 🧪 “Browser inside out” ni tushunmoqchi bo‘lganlar

---

## 🛣️ Roadmap (Vision)
- Binary / Protobuf IPC
- Process isolation & sandbox
- Embedded renderer (WebView2)
- Privacy-first feature flags
- Crash recovery & supervision

Bu repo — **boshlanish nuqtasi**.

---

## ⚠️ Honest Disclaimer
Net-browser:
- production browser emas
- security-hardened emas
- full rendering engine emas

Bu **architecture + integration demo**.  
Shunaqa bo‘lishi ataylab tanlangan.

---

## 📜 License
MIT

---

### ⭐ If this repo helped you
- Star ⭐ qo‘ying
- Fork qiling
- O‘z demo’ngizni quring
