from jinja2_tools_helper.application import handle_request
from jinja2_tools_helper.protocol import Request
def test_valid():assert handle_request(Request(1,"x","validate","x.j2","{% if x %}{{ x }}{% endif %}")).success
def test_missing_endif():assert not handle_request(Request(1,"x","validate","x.j2","{% if x %}")).success
