import time
from dataclasses import dataclass
from cyclonedds.idl import IdlStruct
from cyclonedds.idl.types import uint32
from cyclonedds.core import Listener
from cyclonedds.domain import DomainParticipant
from cyclonedds.topic import Topic
from cyclonedds.pub import DataWriter


@dataclass
class HelloWorld(IdlStruct):
    index: uint32  # matches unsigned long in HelloWorld.idl
    message: str


message = HelloWorld(index=0, message="HelloWorld")


class MyListener(Listener):

    def __init__(self, **kwargs):
        super().__init__(**kwargs)
        self.matched = 0

    def on_publication_matched(self, _, status):
        if status.current_count_change == 1:
            self.matched = self.matched + 1
            print("Publisher matched.")
        elif status.current_count_change == -1:
            self.matched = self.matched - 1
            print("Publisher unmatched.")
        else:
            pass


listener = MyListener()

participant = DomainParticipant()
topic = Topic(participant, "HelloWorldTopic", HelloWorld)
writer = DataWriter(participant, topic, listener=listener)


def publish():
    if listener.matched > 0:
        message.index = message.index + 1
        writer.write(message)
        return True
    return False


def main():
    print("Starting publisher.")
    samples = 10
    samples_sent = 0
    while samples_sent < samples:
        if publish():
            samples_sent = samples_sent + 1
            print("Message: {} with index {} SENT".format(message.message, message.index))
        time.sleep(1)


if __name__ == "__main__":
    main()
