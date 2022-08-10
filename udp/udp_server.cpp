
#include <array>
#include <iostream>
#include <string>

#include <boost/asio.hpp>

using boost::asio::ip::udp;

const int MAX_UDP_PACKET_SIZE = 65508;

int main(int argc, char **argv) {

    int port = 13;
    if (argc == 2) {
        // assuming <port> as input
        try {
            port = std::stoi(argv[1]);
        } catch (const std::exception &ex) {
            std::cerr << "Unable to parse port.." << std::endl;
            return 1;
        }
    }

    try {

        boost::asio::io_service io_service;
        udp::socket socket(io_service, udp::endpoint(udp::v4(), port));

        udp::endpoint remote_endpoint;

        std::string msg;
        std::array<char, MAX_UDP_PACKET_SIZE> recv_buffer{};

        std::function<void()> receive = [&] {
            socket.async_receive_from(boost::asio::buffer(recv_buffer), remote_endpoint, [&](boost::system::error_code, std::size_t len) {
                std::string recv(recv_buffer.begin(), recv_buffer.begin() + static_cast<int>(len));
                msg = "Hello, " + recv + "!";

                socket.async_send_to(boost::asio::buffer(msg), remote_endpoint, [&](boost::system::error_code, std::size_t) {
                    receive();
                });
            });
        };

        receive();

        std::thread t([&] {
            io_service.run();
        });

        std::cout << "Press a key to continue..." << std::endl;
        while (std::cin.get() != '\n') {}

        io_service.stop();
        t.join();

    } catch (std::exception &ex) {
        std::cerr << ex.what() << std::endl;
    }
}
