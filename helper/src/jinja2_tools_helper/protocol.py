from dataclasses import dataclass
from .diagnostics import Diagnostic
PROTOCOL_VERSION=1;OPS={"ping","validate","format","analyze","shutdown"}
@dataclass(frozen=True)
class Request: protocol_version:int;request_id:str;operation:str;name:str;text:str
@dataclass(frozen=True)
class Response:
 request_id:str|None;success:bool;diagnostics:list[Diagnostic];result:object=None
 def to_dict(self):return {"protocolVersion":PROTOCOL_VERSION,"requestId":self.request_id,"success":self.success,"diagnostics":[x.to_dict() for x in self.diagnostics],"result":self.result}
def parse_request(data:dict)->Request:
 if not isinstance(data,dict):raise ValueError("Request must be an object")
 if data.get("protocolVersion")!=PROTOCOL_VERSION:raise ValueError("Unsupported protocol version")
 op=data.get("operation")
 if op not in OPS:raise ValueError(f"Unsupported operation: {op}")
 d=data.get("document") or {};text=d.get("text","")
 if not isinstance(text,str):raise ValueError("document.text must be a string")
 return Request(PROTOCOL_VERSION,str(data.get("requestId","")),op,str(d.get("name","active.j2")),text)
