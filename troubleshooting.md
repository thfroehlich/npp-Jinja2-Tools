# Troubleshooting

## Build tools

### CMake not recognized

Run `where.exe cmake`. Add Visual Studio's `Common7\IDE\CommonExtensions\Microsoft\CMake\CMake\bin` directory to PATH and reopen the terminal.

### VCPKG_ROOT not set or toolchain not found

```powershell
$env:VCPKG_ROOT = "C:\Tools\vcpkg"
Test-Path "$env:VCPKG_ROOT\scripts\buildsystems\vcpkg.cmake"
```

### Missing or invalid baseline

Use a valid 40-character commit. Run:

```powershell
& "$env:VCPKG_ROOT\vcpkg.exe" x-update-baseline --add-initial-baseline
```

Do not use `HEAD`.

### MSB8020 v143 not found

The generator may not match the installed Visual Studio version. Check `cmake --help`, update `CMakePresets.json`, delete `build`, and configure again.

### CMake parse errors

Put `if`, `add_dependencies`, `include` and `catch_discover_tests` on separate lines.

### nlohmann target missing in tests

Add `find_package(nlohmann_json CONFIG REQUIRED)` to `tests/cpp/CMakeLists.txt`.

### Missing ALL_BUILD, no tests, or missing cmake_install.cmake

Fix the first configuration error, delete `build`, then configure again.

## Source compilation

### C2001/C2137/empty character constants

Generated files contained literal control characters. Use escaped literals such as `'\0'`, `L'\0'`, `'\n'`, and `L"\n"`. Previously affected files included Utf8.cpp, Win32Error.cpp, HelperClient.cpp and Diagnostics.cpp.

### std::runtime_error not found

Add `#include <stdexcept>`.

## Helper build

Run manually with:

```powershell
powershell -ExecutionPolicy Bypass -File helper\build-helper.ps1
```

If manual execution succeeds but MSBuild fails, invoke PowerShell with `-NoProfile -ExecutionPolicy Bypass -File` and CMake `VERBATIM`.

## Runtime

### Helper not found

DLL and helper must be directly beside each other in `<Notepad++>\plugins\Jinja2Tools`.

### CreateProcessW error 2

Verify the exact helper path, existence, working directory and command-line quoting. Use:

```cpp
std::wstring commandLine = L"\"" + executable.wstring() + L"\"";
```

A malformed generated command line produced error 2 even when the EXE existed.

### Notepad++ freezes after helper finishes

The parent retained child-side stdout/stderr write handles, so reader threads never observed EOF. Close parent copies immediately after process creation, and close parent stdin-write after sending JSON.

### Invalid control character in JSON

Do not concatenate JSON strings. Use:

```cpp
nlohmann::json requestJson = request;
std::string payload = requestJson.dump();
payload.push_back('\n');
```

### Debug dialogs remain

Remove temporary Helper Path, Command Line, Waiting for helper, CreateProcessW succeeded and Helper finished MessageBox calls before release.

### HTML language is not applied

Verify `PluginCommands.cpp` sends `NPPM_SETCURRENTLANGTYPE` with `L_HTML` after successful replacement. Rebuild and replace the DLL, then restart Notepad++.

### Formatter changes are not visible

Python changes are packaged inside the helper. Rebuild `helper/build-helper.ps1`, replace `Jinja2Tools.Helper.exe`, and restart Notepad++.
