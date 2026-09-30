from jinja2_tools_helper.formatter import FormatterOptions, format_template


def test_formats_compact_html_and_jinja_hierarchically():
    source = "<div>{% if test %}<span>{{ value }}</span>{% endif %}</div>"
    expected = (
        "<div>\n"
        "    {% if test %}\n"
        "        <span>\n"
        "            {{ value }}\n"
        "        </span>\n"
        "    {% endif %}\n"
        "</div>"
    )
    assert format_template(source) == expected


def test_breaks_markup_and_statements_into_lines():
    source = "<section><p>Text</p>{% if x %}<br>{% endif %}</section>"
    expected = (
        "<section>\n"
        "    <p>\n"
        "        Text\n"
        "    </p>\n"
        "    {% if x %}\n"
        "        <br>\n"
        "    {% endif %}\n"
        "</section>"
    )
    assert format_template(source) == expected


def test_uses_four_spaces_by_default():
    lines = format_template("<div><span>{{ value }}</span></div>").splitlines()
    assert lines[1] == "    <span>"
    assert lines[2] == "        {{ value }}"


def test_honors_custom_indent_size():
    lines = format_template(
        "<div><span>x</span></div>", FormatterOptions(indent_size=2)
    ).splitlines()
    assert lines[1] == "  <span>"


def test_void_elements_do_not_increase_indent():
    source = "<div><img src=\"x.png\"><br><span>x</span></div>"
    lines = format_template(source).splitlines()
    assert lines[1] == "    <img src=\"x.png\">"
    assert lines[2] == "    <br>"
    assert lines[3] == "    <span>"


def test_else_and_elif_align_with_if():
    source = "{% if a %}<p>A</p>{% elif b %}<p>B</p>{% else %}<p>C</p>{% endif %}"
    lines = format_template(source).splitlines()
    assert lines[0] == "{% if a %}"
    assert lines[4] == "{% elif b %}"
    assert lines[8] == "{% else %}"
    assert lines[12] == "{% endif %}"


def test_whitespace_control_is_preserved():
    result = format_template("<div>{%- if x -%}{{- x -}}{%- endif -%}</div>")
    assert "{%- if x -%}" in result
    assert "{{- x -}}" in result
    assert "{%- endif -%}" in result


def test_raw_jinja_content_is_not_tokenized():
    raw = "{% raw %}<span>{{ untouched }}</span>{% endraw %}"
    result = format_template(f"<div>{raw}</div>")
    assert raw in result


def test_script_content_is_not_reformatted():
    script = 'if (a < b) { console.log("x"); }'
    result = format_template(f"<script>{script}</script><div>x</div>")
    assert script in result


def test_preserves_crlf():
    result = format_template("<div>\r\n<span>x</span>\r\n</div>\r\n")
    assert "\r\n" in result
    assert "\n" not in result.replace("\r\n", "")


def test_is_idempotent():
    source = "<div>{% if x %}<span>{{ x }}</span>{% endif %}</div>"
    once = format_template(source)
    assert format_template(once) == once
