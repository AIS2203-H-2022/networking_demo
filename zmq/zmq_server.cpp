#include <iostream>
#include <string>
#include <zmqpp/zmqpp.hpp>

int main(int argc, char **argv) {

    std::string port = "4242";
    if (argc == 2) {
        // assuming <port> as input
        port = argv[1];
    }

    const std::string endpoint("tcp://*:" + port);

    // initialize the 0MQ context
    zmqpp::context context;

    // generate a pull socket
    zmqpp::socket_type type = zmqpp::socket_type::pull;
    zmqpp::socket socket(context, type);

    // bind to the socket
    std::cout << "Binding to " << endpoint << "..." << std::endl;
    socket.bind(endpoint);

    // receive the message
    std::cout << "Receiving message..." << std::endl;
    zmqpp::message message;
    // decompose the message
    socket.receive(message);
    std::string text;
    int number;
    message >> text >> number;

    std::cout << "Received text:\"" << text << "\" and a number: " << number << std::endl;
    std::cout << "Finished." << std::endl;
}
