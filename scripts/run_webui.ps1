$ErrorActionPreference = "Stop"
Write-Host "Running WebUI at http://localhost:5173 ..." -ForegroundColor Cyan
Push-Location "src/ui/webui/public"
try {
  if (Get-Command python -ErrorAction SilentlyContinue) {
    python -m http.server 5173
  } else {
    py -m http.server 5173
  }
} finally {
  Pop-Location
}