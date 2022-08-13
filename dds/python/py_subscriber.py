from cyclonedds.domain import DomainParticipant
from cyclonedds.topic import Topic
from cyclonedds.sub import DataReader
from cyclonedds.util import duration

from HelloWorld import HelloWorld

import io
from PIL import Image


def main():
    print("Starting subscriber.")
    participant = DomainParticipant()
    topic = Topic(participant, "HelloWorldTopic", HelloWorld)
    reader = DataReader(participant, topic)

    # If we don't receive a single announcement for three seconds we want the script to exit.
    for msg in reader.take_iter(timeout=duration(seconds=1)):
        stream = io.BytesIO()
        stream.write(bytes(msg.data))
        stream.seek(0)
        img = Image.open(stream)
        print(img.size)
        print("Message RECEIVED")


if __name__ == "__main__":
    main()
