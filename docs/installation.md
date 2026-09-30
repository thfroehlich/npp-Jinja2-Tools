# Installation

Use 64-bit Notepad++ and a 64-bit plugin build. Fully exit Notepad++, then create:

```text
<Notepad++>\plugins\Jinja2Tools\
```

Copy:

```text
Jinja2Tools.dll
Jinja2Tools.Helper.exe
```

The helper must be directly beside the DLL. Recommended additional files are LICENSE, README.md and THIRD_PARTY_NOTICES.md.

Restart Notepad++ and verify `Plugins > Jinja2 Tools`.

For portable Notepad++, use `<portable directory>\plugins\Jinja2Tools`.

When updating, replace the DLL after C++ changes and replace the helper EXE after formatter, protocol, parser or analysis changes. When uncertain, replace both. If Windows shows **Unblock** in file properties, apply it before restarting Notepad++.
