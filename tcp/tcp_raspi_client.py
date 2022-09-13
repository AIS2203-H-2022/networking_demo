import io
import socket
import struct
import cv2 as cv
import numpy as np


def run(host: str, port: int):

    with socket.socket() as sock:
        sock.connect((host, port))

        active_window = False

        with sock.makefile('rb') as conn:

            while True:
                image_len = struct.unpack('<L', conn.read(struct.calcsize('<L')))[0]
                if not image_len:
                    break

                # Construct a stream to hold the image data and read the image
                # data from the connection
                image_stream = io.BytesIO()
                image_stream.write(conn.read(image_len))

                image_stream.seek(0)
                buf = np.asarray(bytearray(image_stream.read()), dtype=np.uint8)

                if not active_window:
                    cv.namedWindow("img")
                    active_window = True

                img = cv.imdecode(buf, cv.IMREAD_COLOR)
                img = cv.resize(img, (640, 480))
                cv.imshow("img", img)
                cv.waitKey(1)


if __name__ == "__main__":
    import sys
    if len(sys.argv) != 3:
        exit(1)

    host = sys.argv[1]
    port = int(sys.argv[2])

    run(host, port)