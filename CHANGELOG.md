# Changelog

## 0.2.0

- Added hierarchical mixed HTML/Jinja formatting with four-space indentation.
- Added structural line breaks for HTML tags and Jinja fragments.
- Preserved raw blocks, whitespace-control delimiters and raw-text HTML elements.
- Added post-format Jinja parsing.
- Added automatic built-in HTML language selection after formatting.
- Replaced manual JSON construction with `nlohmann::json`.
- Corrected helper command-line quoting and pipe-handle ownership.
- Prevented Notepad++ freezes caused by reader threads waiting for EOF.
- Improved Win32 and helper diagnostics.
- Expanded tests and documentation.

## 0.1.0

- Initial prototype.
