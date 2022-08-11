from dataclasses import dataclass
from cyclonedds.idl import IdlStruct
from cyclonedds.domain import DomainParticipant
from cyclonedds.topic import Topic
from cyclonedds.sub import DataReader
from cyclonedds.util import duration
from cyclonedds.idl.types import uint32


@dataclass
class HelloWorld(IdlStruct):
    index: uint32  # matches unsigned long in HelloWorld.idl
    message: str


def main():
    print("Starting subscriber.")
    participant = DomainParticipant()
    topic = Topic(participant, "HelloWorldTopic", HelloWorld)
    reader = DataReader(participant, topic)

    # If we don't receive a single announcement for three seconds we want the script to exit.
    for msg in reader.take_iter(timeout=duration(seconds=3)):
        print("Message: {} with index: {} RECEIVED".format(msg.message, msg.index))


if __name__ == "__main__":
    main()
