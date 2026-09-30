$ErrorActionPreference="Stop"
& "$PSScriptRoot/../helper/build-helper.ps1"
& "$PSScriptRoot/configure.ps1"
cmake --build --preset build-release
ctest --preset test-release
cmake --install "$PSScriptRoot/../build/vs2022-x64-release" --config Release
