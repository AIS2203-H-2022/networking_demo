
#include <thrift/server/TSimpleServer.h>
#include <thrift/transport/TServerSocket.h>
#include <thrift/transport/TTransportUtils.h>

#include "demo/thrift/ThriftDemo.h"

#include <iostream>

using namespace apache::thrift;
using namespace apache::thrift::server;
using namespace apache::thrift::transport;
using namespace apache::thrift::protocol;
using namespace demo::thrift;

const int port = 8080;

class demo_handler : public ThriftDemoIf {
public:
    void greet(std::string &_return, const std::string &name) override {
        _return = "Hello, " + name + "!";
    }
};

int main() {

    std::shared_ptr<ThriftDemoIf> handler = std::make_shared<demo_handler>();
    std::shared_ptr<TProcessor> processor = std::make_shared<ThriftDemoProcessor>(handler);

    std::shared_ptr<TTransportFactory> transportFactory = std::make_shared<TFramedTransportFactory>();
    std::shared_ptr<TProtocolFactory> protocolFactory = std::make_shared<TBinaryProtocolFactory>();

    std::shared_ptr<TServerTransport> serverTransport = std::make_shared<TServerSocket>(port);

    std::unique_ptr<TSimpleServer> server = std::make_unique<TSimpleServer>(processor, serverTransport, transportFactory, protocolFactory);

    std::thread t([&]{
       try {
           server->serve();
       } catch (const std::exception& ex) {
           std::cerr << ex.what() << std::endl;
       }
    });

    std::cout << "Press a key to continue..." << std::endl;
    while (std::cin.get() != '\n') {}

    server->stop();

    if (t.joinable()) {
        t.join();
    }
}
