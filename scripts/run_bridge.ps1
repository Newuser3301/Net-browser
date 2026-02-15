$ErrorActionPreference = "Stop"
Write-Host "Running NetBrowser Bridge (C#)..." -ForegroundColor Cyan
dotnet --version | Out-Host
dotnet run --project "src/bridge/NetBrowser.Bridge/NetBrowser.Bridge.csproj"