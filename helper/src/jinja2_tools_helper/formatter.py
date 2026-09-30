from __future__ import annotations

from dataclasses import dataclass
from enum import Enum
import re

from jinja2 import Environment, TemplateAssertionError, TemplateSyntaxError


@dataclass(frozen=True)
class FormatterOptions:
    indent_size: int = 4
    use_tabs: bool = False
    trim_trailing_whitespace: bool = True
    insert_final_newline: bool = False


class FragmentKind(Enum):
    TEXT = "text"
    HTML_TAG = "html_tag"
    JINJA_VARIABLE = "jinja_variable"
    JINJA_STATEMENT = "jinja_statement"
    JINJA_COMMENT = "jinja_comment"
    RAW_BLOCK = "raw_block"


@dataclass(frozen=True)
class Fragment:
    kind: FragmentKind
    text: str


JINJA_OPEN_BLOCKS = {
    "if", "for", "block", "macro", "call", "filter", "with",
    "autoescape", "trans", "raw",
}
JINJA_MIDDLE_BLOCKS = {"elif", "else", "pluralize"}
JINJA_CLOSE_BLOCKS = {
    "endif", "endfor", "endblock", "endmacro", "endcall",
    "endfilter", "endwith", "endautoescape", "endtrans", "endset",
    "endraw",
}
HTML_VOID_ELEMENTS = {
    "area", "base", "br", "col", "embed", "hr", "img", "input",
    "link", "meta", "param", "source", "track", "wbr",
}
HTML_RAW_TEXT_ELEMENTS = {"script", "style", "pre", "textarea"}
HTML_OPEN_RE = re.compile(r"^<\s*([A-Za-z][\w:.-]*)\b", re.DOTALL)
HTML_CLOSE_RE = re.compile(r"^<\s*/\s*([A-Za-z][\w:.-]*)\s*>", re.DOTALL)


def _advance_quoted(source: str, index: int, quote: str) -> int:
    index += 1
    while index < len(source):
        if source[index] == "\\":
            index += 2
            continue
        if source[index] == quote:
            return index + 1
        index += 1
    return index


def _find_jinja_end(source: str, start: int, delimiter: str) -> int:
    index = start
    quote: str | None = None
    while index < len(source):
        character = source[index]
        if quote is not None:
            if character == "\\":
                index += 2
                continue
            if character == quote:
                quote = None
            index += 1
            continue
        if character in {"'", '"'}:
            quote = character
            index += 1
            continue
        if source.startswith(delimiter, index):
            return index + len(delimiter)
        index += 1
    return len(source)


def _find_html_tag_end(source: str, start: int) -> int:
    index = start + 1
    while index < len(source):
        character = source[index]
        if character in {"'", '"'}:
            index = _advance_quoted(source, index, character)
            continue
        if character == ">":
            return index + 1
        index += 1
    return len(source)


def _statement_name(fragment: str) -> str | None:
    if not fragment.startswith("{%"):
        return None
    body = fragment[2:-2].strip()
    body = body.removeprefix("-").removeprefix("+").strip()
    body = body.removesuffix("-").removesuffix("+").strip()
    return body.split(None, 1)[0] if body else None


def _is_block_set(fragment: str) -> bool:
    if _statement_name(fragment) != "set":
        return False
    body = fragment[2:-2].strip().strip("-+").strip()
    return "=" not in body


def _html_tag_name(fragment: str) -> tuple[str | None, bool]:
    closing = HTML_CLOSE_RE.match(fragment)
    if closing:
        return closing.group(1).lower(), True
    opening = HTML_OPEN_RE.match(fragment)
    if opening:
        return opening.group(1).lower(), False
    return None, False


def _is_html_opening(fragment: str) -> bool:
    name, closing = _html_tag_name(fragment)
    if name is None or closing:
        return False
    stripped = fragment.rstrip()
    return (
        name not in HTML_VOID_ELEMENTS
        and not stripped.endswith("/>")
        and not stripped.startswith("<!")
        and not stripped.startswith("<?")
    )


def _append_text_fragments(output: list[Fragment], text: str) -> None:
    for line in text.splitlines():
        normalized = " ".join(line.split())
        if normalized:
            output.append(Fragment(FragmentKind.TEXT, normalized))


def tokenize_template(source: str) -> list[Fragment]:
    """Split mixed HTML/Jinja source without changing markup tokens."""
    fragments: list[Fragment] = []
    index = 0
    while index < len(source):
        if source.startswith("{#", index):
            end = _find_jinja_end(source, index + 2, "#}")
            fragments.append(Fragment(FragmentKind.JINJA_COMMENT, source[index:end]))
            index = end
            continue
        if source.startswith("{{", index):
            end = _find_jinja_end(source, index + 2, "}}")
            fragments.append(Fragment(FragmentKind.JINJA_VARIABLE, source[index:end]))
            index = end
            continue
        if source.startswith("{%", index):
            end = _find_jinja_end(source, index + 2, "%}")
            statement = source[index:end]
            if _statement_name(statement) == "raw":
                raw_close = re.search(r"{%-?\s*endraw\s*-?%}", source[end:])
                raw_end = len(source) if raw_close is None else end + raw_close.end()
                fragments.append(Fragment(FragmentKind.RAW_BLOCK, source[index:raw_end]))
                index = raw_end
            else:
                fragments.append(Fragment(FragmentKind.JINJA_STATEMENT, statement))
                index = end
            continue
        if source[index] == "<":
            end = _find_html_tag_end(source, index)
            tag = source[index:end]
            fragments.append(Fragment(FragmentKind.HTML_TAG, tag))
            name, closing = _html_tag_name(tag)
            index = end
            if name in HTML_RAW_TEXT_ELEMENTS and not closing and _is_html_opening(tag):
                close_re = re.compile(rf"</\s*{re.escape(name)}\s*>", re.IGNORECASE)
                match = close_re.search(source, index)
                if match is not None:
                    raw_text = source[index:match.start()]
                    if raw_text:
                        fragments.append(Fragment(FragmentKind.RAW_BLOCK, raw_text))
                    fragments.append(Fragment(FragmentKind.HTML_TAG, match.group(0)))
                    index = match.end()
            continue
        positions = [
            position
            for marker in ("{#", "{{", "{%", "<")
            if (position := source.find(marker, index)) >= 0
        ]
        end = min(positions) if positions else len(source)
        _append_text_fragments(fragments, source[index:end])
        index = end
    return fragments


def _indent(level: int, options: FormatterOptions) -> str:
    return "\t" * level if options.use_tabs else " " * (level * options.indent_size)


def format_template(
    source: str,
    options: FormatterOptions = FormatterOptions(),
) -> str:
    """Put HTML/Jinja structural fragments on separate, indented lines."""
    if not source:
        return ""
    newline = "\r\n" if "\r\n" in source else "\n"
    had_final_newline = source.endswith(("\n", "\r"))
    level = 0
    lines: list[str] = []

    for fragment in tokenize_template(source):
        text = fragment.text
        if fragment.kind is FragmentKind.JINJA_STATEMENT:
            name = _statement_name(text)
            if name in JINJA_CLOSE_BLOCKS or name in JINJA_MIDDLE_BLOCKS:
                level = max(0, level - 1)
            lines.append(_indent(level, options) + text.strip())
            if name in JINJA_OPEN_BLOCKS or _is_block_set(text) or name in JINJA_MIDDLE_BLOCKS:
                level += 1
            continue
        if fragment.kind is FragmentKind.HTML_TAG:
            _, closing = _html_tag_name(text)
            if closing:
                level = max(0, level - 1)
            lines.append(_indent(level, options) + text.strip())
            if _is_html_opening(text):
                level += 1
            continue
        if fragment.kind is FragmentKind.RAW_BLOCK:
            raw_lines = text.replace("\r\n", "\n").replace("\r", "\n").split("\n")
            for raw_line in raw_lines:
                if raw_line:
                    lines.append(_indent(level, options) + raw_line)
                elif lines and lines[-1] != "":
                    lines.append("")
            continue
        lines.append(_indent(level, options) + text.strip())

    if options.trim_trailing_whitespace:
        lines = [line.rstrip() for line in lines]
    result = newline.join(lines)
    if (options.insert_final_newline or had_final_newline) and not result.endswith(newline):
        result += newline
    try:
        Environment(keep_trailing_newline=True).parse(result)
    except (TemplateSyntaxError, TemplateAssertionError) as exc:
        raise ValueError(f"FORMAT_VALIDATION_FAILED: {exc}") from exc
    return result
