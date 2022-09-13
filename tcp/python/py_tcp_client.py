import socket


def main():

    # Create a TCP/IP socket
    with socket.socket(socket.AF_INET, socket.SOCK_STREAM) as sock:
        # Connect the socket to the port where the server is listening
        server_address = ('localhost', 9090)
        print("Connecting to {} port {}".format(server_address[0], server_address[1]))
        sock.connect(server_address)

        for i in range(0, 10):
            sock.send("{}\n".format(i).encode("utf-8"))
            data = sock.recv(256).decode("utf-8")[:-1]
            print("Got: {}".format(data))


if __name__ == "__main__":
    main()
