#! python

from thrift import Thrift
from thrift.transport import TSocket
from thrift.transport import TTransport
from thrift.protocol import TBinaryProtocol

from service import ThriftDemo


def main():
    try:
        # Make socket
        transport = TSocket.TSocket('localhost', 8080)

        # Buffering is critical. Raw sockets are very slow
        transport = TTransport.TFramedTransport(transport)

        # Wrap in a protocol
        protocol = TBinaryProtocol.TBinaryProtocol(transport)

        # Create a client to use the protocol encoder
        client = ThriftDemo.Client(protocol)

        # Connect!
        transport.open()

        response = client.greet("Per")

        print("Got: {}".format(response))
    except Exception as err:
        print(err)


if __name__ == "__main__":
    main()
