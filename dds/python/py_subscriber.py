from cyclonedds.domain import DomainParticipant
from cyclonedds.topic import Topic
from cyclonedds.sub import DataReader
from cyclonedds.util import duration

from HelloWorld import HelloWorld

import io
from PIL import Image

import cv2 as cv
import numpy as np

def main():
    print("Starting subscriber.")
    participant = DomainParticipant()
    topic = Topic(participant, "HelloWorldTopic", HelloWorld)
    reader = DataReader(participant, topic)

    cv.namedWindow("img")

    # If we don't receive a single announcement for three seconds we want the script to exit.
    for msg in reader.take_iter(timeout=duration(seconds=1)):

        buf = np.frombuffer(bytes(msg.data), dtype=np.uint8)
        img = cv.imdecode(buf, cv.IMREAD_COLOR)
        cv.imshow("img", img)
        cv.waitKey(10)
        print("Message RECEIVED")


if __name__ == "__main__":
    main()
