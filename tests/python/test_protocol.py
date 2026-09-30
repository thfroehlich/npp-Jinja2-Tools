import pytest
from jinja2_tools_helper.protocol import parse_request
def test_unknown_operation():
 with pytest.raises(ValueError):parse_request({"protocolVersion":1,"requestId":"x","operation":"bad","document":{"name":"x","text":""}})
