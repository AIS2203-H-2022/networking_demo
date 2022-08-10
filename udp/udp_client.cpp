
#include <boost/asio.hpp>

#include <array>
#include <iostream>

using boost::asio::ip::udp;

const int port = 13;
const char* host = "127.0.0.1"; // loopback address, i.e. "localhost"
const int MAX_UDP_PACKET_SIZE = 65508;

int main() {

    try {

        std::string message = "Per";

        boost::asio::io_service io_service;
        udp::socket socket(io_service);
        socket.open(udp::v4());

        udp::endpoint receiver_endpoint(boost::asio::ip::address::from_string(host), 13);
        socket.send_to(boost::asio::buffer(message), receiver_endpoint);

        udp::endpoint sender_endpoint;
        std::array<char, 256> recv_buf{};
        auto len = socket.receive_from(boost::asio::buffer(recv_buf), sender_endpoint);
        std::string recv(recv_buf.begin(), recv_buf.begin() + static_cast<int>(len));

        std::cout << "Server responded with: " << recv  << std::endl;

    } catch (std::exception& e) {
        std::cerr << e.what() << std::endl;
    }

}