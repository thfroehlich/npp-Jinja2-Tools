from .tokens import Token, TokenKind


def scan_template(source: str) -> list[Token]:
    output: list[Token] = []
    index = 0
    line = 1
    column = 1
    raw = False

    def emit(kind: TokenKind, start: int, end: int, token_line: int, token_column: int) -> None:
        output.append(Token(kind, source[start:end], start, end, token_line, token_column))

    while index < len(source):
        token_line = line
        token_column = column
        if raw:
            end = source.find("{% endraw", index)
            if end < 0:
                end = len(source)
            else:
                closing = source.find("%}", end)
                end = len(source) if closing < 0 else closing + 2
            emit(TokenKind.RAW_BLOCK, index, end, token_line, token_column)
            chunk = source[index:end]
            line += chunk.count("\n")
            column = len(chunk.rsplit("\n", 1)[-1]) + 1
            index = end
            raw = False
            continue

        choices = [
            (source.find(opening, index), opening, kind)
            for opening, kind in (("{{", TokenKind.VARIABLE), ("{%", TokenKind.STATEMENT), ("{#", TokenKind.COMMENT))
            if source.find(opening, index) >= 0
        ]
        if not choices:
            emit(TokenKind.TEXT, index, len(source), token_line, token_column)
            break

        position, opening, kind = min(choices)
        if position > index:
            emit(TokenKind.TEXT, index, position, token_line, token_column)
            chunk = source[index:position]
            line += chunk.count("\n")
            column = len(chunk.rsplit("\n", 1)[-1]) + 1
            index = position
            token_line = line
            token_column = column

        close = {"{{": "}}", "{%": "%}", "{#": "#}"}[opening]
        cursor = index + 2
        quote: str | None = None
        while cursor < len(source) - 1:
            character = source[cursor]
            if quote:
                if character == "\\":
                    cursor += 2
                    continue
                if character == quote:
                    quote = None
            elif character in "'\"":
                quote = character
            elif source.startswith(close, cursor):
                cursor += 2
                break
            cursor += 1

        emit(kind, index, cursor, token_line, token_column)
        chunk = source[index:cursor]
        line += chunk.count("\n")
        column = len(chunk.rsplit("\n", 1)[-1]) + 1
        if kind is TokenKind.STATEMENT:
            parts = chunk[2:-2].strip(" -").split(None, 1)
            raw = bool(parts and parts[0] == "raw")
        index = cursor
    return output
