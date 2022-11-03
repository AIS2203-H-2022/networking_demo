#include <atomic>
#include <iostream>
#include <memory>
#include <string>
#include <thread>

#include "network_helper.hpp"

#include <boost/asio.hpp>

using namespace boost::asio;
using namespace boost::asio::ip;


class socket_handler {

public:
    explicit socket_handler(std::unique_ptr<tcp::socket> socket)
        : socket_(std::move(socket)) {
    }

    socket_handler(const socket_handler &) = delete;
    socket_handler(const socket_handler &&) = delete;

    void run() {
        t_ = std::thread([this] {
            try {
                for (;;) {
                    std::string data = recvMsg();
                    std::string msg = "Hello, " + data + "!";
                    int msgSize = static_cast<int>(msg.size());

                    socket_->send(boost::asio::buffer(int_to_bytes(msgSize), 4));
                    socket_->send(boost::asio::buffer(msg));
                }
            } catch (const std::exception &ex) {
                std::cerr << "[socket_handler] " << ex.what() << std::endl;
            }
        });
    }

    ~socket_handler() {
        if (t_.joinable()) {
            t_.join();
        }
    }

private:
    std::thread t_;
    std::unique_ptr<tcp::socket> socket_;

    int recvSize() {
        std::array<unsigned char, 4> buf{};
        boost::asio::read(*socket_, boost::asio::buffer(buf), boost::asio::transfer_exactly(4));
        return bytes_to_int(buf);
    }

    std::string recvMsg() {

        int len = recvSize();

        boost::asio::streambuf buf;
        boost::system::error_code err;
        boost::asio::read(*socket_, buf, boost::asio::transfer_exactly(len), err);

        if (err) {
            throw boost::system::system_error(err);
        }

        std::string data(boost::asio::buffer_cast<const char *>(buf.data()), len);
        return data;
    }
};

int main(int argc, char **argv) {

    int port = 9090;
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
        io_service io_service;
        tcp::acceptor acceptor(io_service, tcp::endpoint(tcp::v4(), port));

        std::cout << "Serving TCP connections on port " << port << std::endl;

        std::atomic<bool> stop = false;
        std::thread t([&] {
            std::vector<std::unique_ptr<socket_handler>> handlers;
            try {
                while (!stop) {
                    auto socket = std::make_unique<tcp::socket>(io_service);
                    acceptor.accept(*socket);// blocking operation

                    handlers.emplace_back(std::make_unique<socket_handler>(std::move(socket)));
                    handlers.back()->run();
                }
            } catch (const std::exception &ex) {
                std::cerr << ex.what() << std::endl;
            }
        });

        std::cout << "Press a key to continue..." << std::endl;
        while (std::cin.get() != '\n') {}

        stop = true;
        acceptor.close();

        if (t.joinable()) {
            t.join();
        }
    } catch (const std::exception &ex) {
        std::cerr << ex.what() << std::endl;
    }
}
