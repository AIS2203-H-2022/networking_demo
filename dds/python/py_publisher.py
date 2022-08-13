import time

from cyclonedds.core import Listener
from cyclonedds.domain import DomainParticipant
from cyclonedds.topic import Topic
from cyclonedds.pub import DataWriter

import io
from PIL import Image

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


def image_to_byte_array(image: Image) -> bytes:
    # BytesIO is a fake file stored in memory
    imgByteArr = io.BytesIO()
    # image.save expects a file as a argument, passing a bytes io ins
    image.save(imgByteArr, format=image.format)
    # Turn the BytesIO object back into a bytes object
    imgByteArr = imgByteArr.getvalue()
    return imgByteArr


class HelloWorldPublisher:

    def __init__(self):
        participant = DomainParticipant()
        topic = Topic(participant, "HelloWorldTopic", HelloWorld)

        image = Image.open("Lenna.png")
        bytes = image_to_byte_array(image)
        self.listener = MyListener()
        self.writer = DataWriter(participant, topic, listener=self.listener)
        self.message = HelloWorld(data=list(bytes))

    def __publish(self) -> bool:
        if self.listener.matched > 0:
            # self.message.index = self.message.index + 1
            self.writer.write(self.message)
            return True
        return False

    def run(self, num_samples: int):
        samples_sent = 0
        while samples_sent < num_samples:
            if self.__publish():
                samples_sent = samples_sent + 1
                print("Message SENT")
            else:
                time.sleep(1)


def main():
    print("Starting publisher.")

    pub = HelloWorldPublisher()
    pub.run(10)


if __name__ == "__main__":
    main()
