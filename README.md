# Jinja2 Tools for Notepad++

Jinja2 Tools is a 64-bit Notepad++ plugin for formatting, validating and statically analyzing Jinja2 templates. A bundled Python helper performs Jinja2 parsing, mixed HTML/Jinja formatting and analysis.


## Features

- Format a complete document or current selection.
- Validate syntax with the Jinja2 parser.
- Analyze variables, blocks, macros, filters, tests and dependencies.
- Format mixed HTML/Jinja structures with four spaces per level.
- Preserve Jinja raw blocks, whitespace-control delimiters and raw-text HTML elements.
- Apply the built-in Notepad++ HTML language after successful formatting.
- Keep formatting changes in one undo action.


## Commands

`Plugins > Jinja2 Tools` contains: Format Document, Format Selection, Check Syntax, Analyze Template, Clear Diagnostics and About.


## Quick build

```powershell
$env:VCPKG_ROOT = "C:\Tools\vcpkg"
powershell -ExecutionPolicy Bypass -File scripts\build.ps1
```

Use the Visual Studio generator installed on the build machine in `CMakePresets.json`, for example `Visual Studio 18 2026`.

## Automated releases
Push a semantic version tag to build and publish a release ZIP:
```powershell
git tag v0.2.0
git push origin v0.2.0
```
See `docs/github-release.md`.


## Installation

```text
<Notepad++>\plugins\Jinja2Tools\
├── Jinja2Tools.dll
└── Jinja2Tools.Helper.exe
```

Fully exit and restart Notepad++ after replacing files.


## Documentation

- [Build](docs/build.md)
- [Installation](docs/installation.md)
- [User guide](docs/user-guide.md)
- [Architecture](docs/architecture.md)
- [Formatter](docs/formatter.md)
- [Protocol](docs/protocol.md)
- [Troubleshooting](docs/troubleshooting.md)
- [Release checklist](docs/release-checklist.md)
