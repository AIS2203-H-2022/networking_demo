from dataclasses import dataclass
from cyclonedds.idl import IdlStruct
from cyclonedds.idl.types import sequence, char


@dataclass
class HelloWorld(IdlStruct):
    data: str
