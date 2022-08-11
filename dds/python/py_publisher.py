import time

from cyclonedds.core import Listener
from cyclonedds.domain import DomainParticipant
from cyclonedds.topic import Topic
from cyclonedds.pub import DataWriter

from HelloWorld import HelloWorld


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


class HelloWorldPublisher:

    def __init__(self):
        participant = DomainParticipant()
        topic = Topic(participant, "HelloWorldTopic", HelloWorld)

        self.listener = MyListener()
        self.writer = DataWriter(participant, topic, listener=self.listener)
        self.message = HelloWorld(index=0, message="HelloWorld")

    def __publish(self) -> bool:
        if self.listener.matched > 0:
            self.message.index = self.message.index + 1
            self.writer.write(self.message)
            return True
        return False

    def run(self, num_samples: int):
        samples_sent = 0
        while samples_sent < num_samples:
            if self.__publish():
                samples_sent = samples_sent + 1
                print("Message: {} with index {} SENT".format(self.message.message, self.message.index))
            time.sleep(1)


def main():
    print("Starting publisher.")

    pub = HelloWorldPublisher()
    pub.run(10)


if __name__ == "__main__":
    main()
