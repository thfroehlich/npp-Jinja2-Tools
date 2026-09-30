from dataclasses import dataclass,asdict
@dataclass(frozen=True)
class Diagnostic:
 severity:str;code:str;message:str;source:str="helper";line:int|None=None;column:int|None=None;positionAccuracy:str|None=None;exceptionType:str|None=None
 def to_dict(self):return {k:v for k,v in asdict(self).items() if v is not None}
