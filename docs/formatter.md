# Formatter

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

HTML tags, Jinja statements, expressions, comments and text nodes are placed on structural lines. Combined HTML/Jinja nesting uses four spaces per level. Closing elements and end-tags reduce the level before output. `elif`, `else` and `pluralize` align with their opening block.

HTML void elements and `/>` tags do not increase nesting. Jinja raw blocks and `script`, `style`, `pre`, and `textarea` content are protected. CRLF/LF and existing final newlines are preserved. Whitespace-control delimiters remain unchanged.

The result is parsed again with Jinja2. Invalid formatter output produces `FORMAT_VALIDATION_FAILED` and must not replace the document.
