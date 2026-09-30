import json
import sys

from jinja2_tools_helper.application import handle_request
from jinja2_tools_helper.diagnostics import Diagnostic
from jinja2_tools_helper.protocol import Response, parse_request


def main() -> int:
    if sys.stdin is None:
        print(
        "This executable is intended to be launched by "
        "the Jinja2Tools plugin."
        )
        return 1
    raw = sys.stdin.buffer.readline()
    request_id = None
    try:
        data = json.loads(raw.decode("utf-8"))
        request_id = data.get("requestId") if isinstance(data, dict) else None
        response = handle_request(parse_request(data))
    except json.JSONDecodeError as exc:
        response = Response(request_id, False, [Diagnostic("error", "JSON_PARSE_ERROR", str(exc))])
    except Exception as exc:
        response = Response(request_id, False, [Diagnostic("error", "PROTOCOL_ERROR", str(exc), exceptionType=type(exc).__name__)])
    payload = json.dumps(response.to_dict(), ensure_ascii=False, separators=(",", ":")) + "\n"
    sys.stdout.buffer.write(payload.encode("utf-8"))
    sys.stdout.buffer.flush()
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
