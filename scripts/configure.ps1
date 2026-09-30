$ErrorActionPreference="Stop"
if(!$env:VCPKG_ROOT){throw "VCPKG_ROOT is not set."}
if(!(Test-Path "$env:VCPKG_ROOT/scripts/buildsystems/vcpkg.cmake")){throw "vcpkg toolchain not found."}
cmake --preset vs2022-x64-release
