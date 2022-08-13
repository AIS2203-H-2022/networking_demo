from dataclasses import dataclass
from cyclonedds.idl import IdlStruct
from cyclonedds.idl.types import sequence, uint8


@dataclass
class HelloWorld(IdlStruct):
    data: sequence[uint8]
