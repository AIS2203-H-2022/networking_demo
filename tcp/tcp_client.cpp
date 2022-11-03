
#include <boost/asio.hpp>

#include <array>
#include <iostream>
#include <string>

using boost::asio::ip::tcp;

namespace {

    std::array<unsigned char, 4> int_to_bytes(int n) {
        std::array<unsigned char, 4> bytes{};

        bytes[0] = (n >> 24) & 0xFF;
        bytes[1] = (n >> 16) & 0xFF;
        bytes[2] = (n >> 8) & 0xFF;
        bytes[3] = n & 0xFF;

        return bytes;
    }

}

int main(int argc, char **argv) {

    std::string host = "127.0.0.1";
    std::string port = "9090";
    if (argc == 3) {
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
            std::string msg = std::to_string(i);
            int msgSize = static_cast<int>(msg.size());

            socket.send(boost::asio::buffer(int_to_bytes(msgSize), 4));
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