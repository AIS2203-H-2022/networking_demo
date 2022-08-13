import time

from cyclonedds.core import Listener
from cyclonedds.domain import DomainParticipant
from cyclonedds.topic import Topic
from cyclonedds.pub import DataWriter

from threading import Thread

import cv2 as cv

from ImageStruct import ImageStruct


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
        topic = Topic(participant, "HelloWorldTopic", ImageStruct)

        self.vid = cv.VideoCapture()
        if not self.vid.open(0):
            self.vid = None

        self.stop = False

        self.listener = MyListener()
        self.writer = DataWriter(participant, topic, listener=self.listener)

    def __publish(self) -> bool:
        if self.listener.matched > 0:
            if self.vid is not None:
                img = self.vid.read()[1]
                encode_param = [int(cv.IMWRITE_JPEG_QUALITY), 90]
                img = cv.imencode('.jpg', img, encode_param)[1]
            else:
                img = cv.imread("Lenna.png")
            message = ImageStruct(img.tolist())
            self.writer.write(message)
            return True
        return False

    def run(self):
        while not self.stop:
            if self.__publish():
                print("Message SENT")
            else:
                time.sleep(1)
        if self.vid is not None:
            self.vid.release()


def main():
    print("Starting publisher.")

    pub = HelloWorldPublisher()
    t = Thread(target=pub.run)
    t.start()

    input("Press Enter to continue...\n")
    pub.stop = True


if __name__ == "__main__":
    main()
