import socket


def main():

    # Create a TCP/IP socket
    with socket.socket(socket.AF_INET, socket.SOCK_STREAM) as sock:
        # Connect the socket to the port where the server is listening
        server_address = ('localhost', 9090)
        print("Connecting to {} port {}".format(server_address[0], server_address[1]))
        sock.connect(server_address)

        for i in range(0, 10):
            msg = "{}".format(i).encode("utf-8")

            msgSize = len(msg)
            sock.send(msgSize.to_bytes(4, byteorder="little"))
            sock.send(msg)

            msgSize = int.from_bytes(sock.recv(4), byteorder="little")
            data = sock.recv(msgSize).decode("utf-8")
            print("Got: {}".format(data))


if __name__ == "__main__":
    main()
