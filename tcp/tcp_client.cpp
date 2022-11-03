
#include <boost/asio.hpp>

#include <array>
#include <iostream>
#include <string>

#include "network_helper.hpp"

using boost::asio::ip::tcp;

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

            boost::system::error_code error;

            std::array<unsigned char, 4> sizeBuf{};
            boost::asio::read(socket, boost::asio::buffer(sizeBuf), boost::asio::transfer_exactly(4), error);
            if (error) {
                throw boost::system::system_error(error);
            }

            boost::asio::streambuf buf;
            size_t len = boost::asio::read(socket, buf, boost::asio::transfer_exactly(bytes_to_int(sizeBuf)), error);
            if (error) {
                throw boost::system::system_error(error);
            }

            std::string data(boost::asio::buffer_cast<const char *>(buf.data()), len);
            std::cout << "Got reply from server: " << data << std::endl;
        }

    } catch (const std::exception &e) {
        std::cerr << e.what() << std::endl;
    }
}