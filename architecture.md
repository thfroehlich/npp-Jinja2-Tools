# Architecture

## Components

`Jinja2Tools.dll` integrates with Notepad++ and Scintilla. `Jinja2Tools.Helper.exe` is a one-shot Python/Jinja2 process packaged with PyInstaller.

The DLL contains PluginContext, PluginCommands, EditorService, HelperClient, HelperProcess, Protocol, Diagnostics, Utf8 and Win32Error services. The helper contains the protocol models, scanner, formatter, parser and analyzer.

## Helper lifecycle

1. Create stdin, stdout and stderr pipes.
2. Start the helper with a correctly quoted `CreateProcessW` command line.
3. Close the parent's copies of the child-side pipe handles.
4. Write one UTF-8 JSON request and close parent stdin.
5. Drain stdout and stderr concurrently.
6. Wait with a timeout and validate the response.

Closing the parent copies of child stdout/stderr write handles is required. Leaving them open prevents EOF, blocks reader-thread joins and freezes Notepad++.

## Formatting flow

The plugin captures editor text, calls the helper, tokenizes mixed HTML/Jinja, formats with four spaces per combined nesting level, validates with `Environment.parse`, replaces text in one undo action, and sends `NPPM_SETCURRENTLANGTYPE` with `L_HTML`.

## Limitations

The formatter is structural rather than a complete browser-grade HTML parser. Diagnostics currently use modal dialogs. Application-specific Jinja filters and globals are not executed.
