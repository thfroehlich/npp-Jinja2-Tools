$ErrorActionPreference="Stop"
& "$PSScriptRoot/../helper/.venv/Scripts/python.exe" -m pytest "$PSScriptRoot/../tests/python"
ctest --preset test-release
