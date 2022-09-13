
from dataclasses import dataclass
from cyclonedds.idl import IdlStruct
from cyclonedds.idl.types import uint32


@dataclass
class HelloWorld(IdlStruct):
    index: uint32  # matches unsigned long in HelloWorld.idl
    message: str
