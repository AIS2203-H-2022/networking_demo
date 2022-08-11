import socket


def main():
    # Create a TCP/IP socket
    sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)

    try:
        # Connect the socket to the port where the server is listening
        server_address = ('localhost', 9090)
        print("Connecting to {} port {}".format(server_address[0], server_address[1]))
        sock.connect(server_address)

        sock.send("Per\n".encode("utf-8"))
        data = sock.recv(256)

        print("Got: {}".format(data.decode("utf-8")))
    finally:
        sock.close()


if __name__ == "__main__":
    main()
