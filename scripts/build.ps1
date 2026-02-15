$ErrorActionPreference = "Stop"
Write-Host "Building Net-browser core (CMake)..." -ForegroundColor Cyan

$root = (Resolve-Path (Join-Path $PSScriptRoot "..")).Path
$build = Join-Path $root "build"
if (-not (Test-Path $build)) { New-Item -ItemType Directory -Path $build | Out-Null }

Push-Location $build
try {
  $gen = "Visual Studio 17 2022"
  cmake .. -G $gen -A x64 | Out-Host
  cmake --build . --config Release | Out-Host
} finally {
  Pop-Location
}

Write-Host "Done ✅" -ForegroundColor Green