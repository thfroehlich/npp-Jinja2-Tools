# Build guide

## Prerequisites

Install Visual Studio with Desktop development with C++, MSVC, Windows SDK, CMake tools, Git, PowerShell, Python 3.11+ and vcpkg.

Verify:

```powershell
cmake --version
python --version
git --version
```

In a Developer Command Prompt verify `cl` and `msbuild`.

## vcpkg

```powershell
$env:VCPKG_ROOT = "C:\Tools\vcpkg"
Test-Path "$env:VCPKG_ROOT\scripts\buildsystems\vcpkg.cmake"
```

Use manifest mode. `vcpkg.json` requires a valid 40-character `builtin-baseline`. Initialize it with:

```powershell
& "$env:VCPKG_ROOT\vcpkg.exe" x-update-baseline --add-initial-baseline
```

Do not use `HEAD` as the baseline value.

## Generator

Run `cmake --help` and set the installed Visual Studio generator in `CMakePresets.json`, for example:

```json
"generator": "Visual Studio 18 2026"
```

Delete `build` after changing generator, compiler, toolchain or baseline:

```powershell
Remove-Item build -Recurse -Force -ErrorAction SilentlyContinue
```

## Configure

```powershell
cmake --preset vs2022-x64-release
```

A successful configure only creates build files.

## Build helper

```powershell
powershell -ExecutionPolicy Bypass -File helper\build-helper.ps1
```

Expected: `helper\dist\Jinja2Tools.Helper.exe`.

## Build plugin

```powershell
cmake --build --preset build-release
```

Expected: `build\vs2022-x64-release\plugin\Release\Jinja2Tools.dll`.

## Tests

```powershell
$env:PYTHONPATH = "helper/src"
python -m pytest tests\python
ctest --preset test-release
```

## Install tree

```powershell
cmake --install build\vs2022-x64-release --config Release
```

## Complete build

```powershell
powershell -ExecutionPolicy Bypass -File scripts\build.ps1
```

If configuration fails, do not continue with build/test/install. Missing `ALL_BUILD.vcxproj`, no tests and missing `cmake_install.cmake` are usually follow-on errors.

`tests/cpp/CMakeLists.txt` must discover both Catch2 and nlohmann-json. Keep separate CMake commands on separate lines.
