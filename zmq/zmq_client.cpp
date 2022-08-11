#include <iostream>
#include <string>
#include <zmqpp/zmqpp.hpp>

int main(int argc, char **argv) {

    std::string host = "localhost";
    std::string port = "4242";
    if (argc == 3) {
        // assuming <host> <port> as input
        host = argv[1];
        port = argv[2];
    }
    const std::string endpoint("tcp://" + host + ":" + port);

    // initialize the 0MQ context
    zmqpp::context context;

    // generate a push socket
    zmqpp::socket_type type = zmqpp::socket_type::push;
    zmqpp::socket socket(context, type);

    // open the connection
    std::cout << "Opening connection to " << endpoint << "..." << std::endl;
    socket.connect(endpoint);

    // send a message
    std::cout << "Sending text and a number..." << std::endl;
    zmqpp::message message;
    // compose a message from a string and a number
    message << "Hello World!" << 42;
    socket.send(message);

    std::cout << "Sent message." << std::endl;
    std::cout << "Finished." << std::endl;
}
