from dataclasses import dataclass
from enum import Enum
class TokenKind(Enum): TEXT="text"; VARIABLE="variable"; STATEMENT="statement"; COMMENT="comment"; RAW_BLOCK="raw_block"
@dataclass(frozen=True)
class Token: kind:TokenKind; text:str; start:int; end:int; line:int; column:int
