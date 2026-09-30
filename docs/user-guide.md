# User guide

Jinja2 Tools formats and checks Jinja2 templates in Notepad++.

## Format Document

Formats the active document into hierarchical HTML/Jinja layout using four spaces per level. Raw content and whitespace-control delimiters are preserved. The result is validated before replacement, the edit is one undo action, and Notepad++ switches to the built-in HTML language.

## Format Selection

Select a complete HTML/Jinja fragment and choose **Format Selection**. Only the selected range is replaced. With no selection, the plugin displays an informational message.

## Check Syntax

Parses the document with Jinja2 without rendering it. Success is confirmed; errors are shown as structured diagnostics. A reported column may be estimated when the parser provides only a line.

## Analyze Template

Reports undeclared variables, blocks, macros, filters, tests, static dependencies and whether dynamic dependencies exist. Dynamic dependencies are not guessed.

## Clear Diagnostics

Clears editor diagnostic indicators managed by the plugin.

## About

Displays plugin name and version.

## HTML highlighting

After successful formatting, the plugin applies Notepad++'s built-in HTML language. No custom UDL is required. Jinja syntax is not fully understood by the native HTML lexer.

## Example

Input:

```jinja2
<div>{% if customer %}<span>{{ customer.name }}</span>{% endif %}</div>
```

Output:

```jinja2
<div>
    {% if customer %}
        <span>
            {{ customer.name }}
        </span>
    {% endif %}
</div>
```

## Updating

Exit Notepad++ before replacing files. Replace the DLL for native changes. Replace the helper EXE for formatter/parser/analyzer changes. Replace both when uncertain, then restart Notepad++.
