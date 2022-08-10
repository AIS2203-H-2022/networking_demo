
#include <boost/asio.hpp>

#include <array>
#include <iostream>
#include <string>

using boost::asio::ip::tcp;

int main(int argc, char** argv) {

    std::string host = "127.0.0.1";
    std::string port = "9090";
    if(argc == 3) {
        // assuming <hostname> <port>
        host = argv[1];
        port = argv[2];
    }

    try {
        boost::asio::io_service io_service;

        tcp::resolver resolver(io_service);
        auto endpoints = resolver.resolve(host, port);

        tcp::socket socket(io_service);
        boost::asio::connect(socket, endpoints);

        for (int i = 0; i < 10; i++) {
            std::string msg = std::to_string(i) + "\n";
            socket.send(boost::asio::buffer(msg));

            std::array<char, 256> buf{};
            boost::system::error_code error;

            size_t len = socket.read_some(boost::asio::buffer(buf), error);

            if (error) {
                throw boost::system::system_error(error);
            }

            std::cout.write(buf.data(), static_cast<std::streamsize>(len));
        }

    } catch (std::exception &e) {
        std::cerr << e.what() << std::endl;
    }
}