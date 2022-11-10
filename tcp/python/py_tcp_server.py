import socket


def main():
    try:
        with socket.socket(socket.AF_INET, socket.SOCK_STREAM) as sock:
            # Connect the socket to the port where the server is listening
            server_address = ('localhost', 9090)
            print("Serving connections on port {}".format(server_address[1]))
            sock.bind(server_address)

            sock.listen()
            conn, addr = sock.accept()
            with conn:
                print(f"Connected by {addr}")
                while True:
                    msgSize = int.from_bytes(conn.recv(4), byteorder="little")
                    data = conn.recv(msgSize).decode("utf-8")

                    msg = "Hello, {}!".format(data).encode("utf-8")
                    conn.send(len(msg).to_bytes(4, byteorder="little"))
                    conn.send(msg)
    except Exception as e:
        print(e)


if __name__ == "__main__":
    main()
