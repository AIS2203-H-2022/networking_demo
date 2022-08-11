
#include <thrift/protocol/TBinaryProtocol.h>
#include <thrift/transport/TSocket.h>
#include <thrift/transport/TTransportUtils.h>

#include "demo/thrift/ThriftDemo.h"

#include <iostream>

using namespace apache::thrift;
using namespace apache::thrift::protocol;
using namespace apache::thrift::transport;
using namespace demo::thrift;


int main(int argc, char **argv) {

    std::string host = "localhost";
    int port = 8080;
    if (argc == 3) {
        // assuming <hostname> <port>
        host = argv[1];
        try {
            port = std::stoi(argv[2]);
        } catch (const std::exception &ex) {
            std::cerr << "Unable to parse port.." << std::endl;
            return 1;
        }
    }

    try {
        std::shared_ptr<TTransport> socket = std::make_shared<TSocket>(host, port);
        std::shared_ptr<TTransport> transport = std::make_shared<TFramedTransport>(socket);
        std::shared_ptr<TProtocol> protocol = std::make_shared<TBinaryProtocol>(transport);
        std::shared_ptr<ThriftDemoClient> client = std::make_shared<ThriftDemoClient>(protocol);

        transport->open();

        std::string response;
        client->greet(response, "Nils");

        std::cout << "Got: " << response << std::endl;
    } catch (const std::exception &ex) {
        std::cerr << "Error: " << ex.what() << std::endl;
    }
}