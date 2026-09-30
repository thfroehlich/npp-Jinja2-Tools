# Helper protocol

Protocol version 1 uses one UTF-8 JSON request line on stdin and one UTF-8 JSON response line on stdout. stdout is reserved for JSON; stderr is captured separately.

Request:

```json
{"protocolVersion":1,"requestId":"id","operation":"format","document":{"name":"active.j2","text":"<div>{{ value }}</div>"}}
```

Supported operations: `ping`, `validate`, `format`, `analyze`, `shutdown`.

Response:

```json
{"protocolVersion":1,"requestId":"id","success":true,"diagnostics":[],"result":{"formatted":"..."}}
```

The native plugin uses only `nlohmann::json`. It validates protocol version, request ID, exit code, timeout, non-empty stdout and valid JSON.

After process creation, the parent closes its copies of child stdin-read, stdout-write and stderr-write handles. After writing the request, the parent closes stdin-write. This is required for EOF and avoids reader-thread deadlocks.
