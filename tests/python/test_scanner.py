from jinja2_tools_helper.scanner import scan_template


def test_scanner():
    assert [token.kind.value for token in scan_template("A {{ x }} {# c #}")] == ["text", "variable", "text", "comment"]
