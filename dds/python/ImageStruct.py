from dataclasses import dataclass
from cyclonedds.idl import IdlStruct
from cyclonedds.idl.types import sequence, uint8


@dataclass
class ImageStruct(IdlStruct):
    data: sequence[uint8]
