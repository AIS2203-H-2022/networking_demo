from cyclonedds.domain import DomainParticipant
from cyclonedds.topic import Topic
from cyclonedds.sub import DataReader
from cyclonedds.util import duration

from ImageStruct import ImageStruct

import cv2 as cv
import numpy as np


def main():
    print("Starting subscriber.")
    participant = DomainParticipant()
    topic = Topic(participant, "RaspiImageTopic", ImageStruct)
    reader = DataReader(participant, topic)

    display = False

    # If we don't receive a single announcement for three seconds we want the script to exit.
    for msg in reader.take_iter(timeout=duration(seconds=3)):

        if not display:
            cv.namedWindow("img")
            display = True

        buf = np.frombuffer(bytes(msg.data), dtype=np.uint8)
        img = cv.imdecode(buf, cv.IMREAD_COLOR)
        #img = cv.resize(img, (640, 480))
        cv.imshow("img", img)
        cv.waitKey(1)
        #print("Message RECEIVED")


if __name__ == "__main__":
    main()
