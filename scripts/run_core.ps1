$ErrorActionPreference = "Stop"
$root = (Resolve-Path (Join-Path $PSScriptRoot "..")).Path
$exe  = Join-Path $root "build\Release\netbrowser_core.exe"
if (-not (Test-Path $exe)) {
  throw "Core not built. Run scripts/build.ps1 first."
}
& $exe