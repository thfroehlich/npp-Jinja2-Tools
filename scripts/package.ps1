$ErrorActionPreference="Stop"
& "$PSScriptRoot/build.ps1"
cmake --build "$PSScriptRoot/../build/vs2022-x64-release" --config Release --target package
Get-FileHash "$PSScriptRoot/../build/vs2022-x64-release/Jinja2Tools-0.1.0-win64.zip" -Algorithm SHA256 | Out-File "$PSScriptRoot/../dist/Jinja2Tools-0.1.0-win64.zip.sha256"
