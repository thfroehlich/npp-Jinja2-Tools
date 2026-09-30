# Release checklist

## Source

- [ ] Version and changelog updated.
- [ ] No temporary debug MessageBox calls remain.
- [ ] No compressed one-line C++ files or embedded control characters remain.
- [ ] `PluginCommands.cpp` applies `L_HTML` after formatting.
- [ ] `HelperProcess.cpp` closes parent copies of child-side handles.
- [ ] JSON uses only `nlohmann::json`.

## Build and tests

- [ ] Valid vcpkg baseline and pinned dependencies.
- [ ] Clean configuration succeeds.
- [ ] Release build succeeds.
- [ ] DLL and helper EXE exist.
- [ ] Python tests pass.
- [ ] C++ tests and CTest pass.

## Manual validation

- [ ] Plugin menu appears.
- [ ] Document and selection formatting work.
- [ ] One undo restores formatting.
- [ ] Built-in HTML language is selected.
- [ ] Valid/invalid syntax checks work.
- [ ] Analysis returns expected data.
- [ ] Raw blocks, whitespace control, CRLF and Unicode are preserved.
- [ ] Notepad++ does not freeze.
- [ ] No helper remains after completion.

## Package

- [ ] DLL and helper are side by side.
- [ ] No virtual environment, caches or temporary build files are included.
- [ ] Checksum and third-party notices are present.
- [ ] Installation is tested in a clean portable Notepad++ copy.
