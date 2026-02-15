# Build (Windows)

## Core (C++)
From VS Developer PowerShell:
- mkdir build
- cd build
- cmake .. -G ""Visual Studio 17 2022""
- cmake --build . --config Release
Run:
- .\\Release\\netbrowser_core.exe

## GUI (C# WPF)
Open:
- src/ui/windows_wpf/NetBrowser.Shell/NetBrowser.Shell.csproj
Press Run (needs WebView2 runtime).