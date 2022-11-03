

#ifndef NETWORKING_DEMO_NETWORK_HELPER_HPP
#define NETWORKING_DEMO_NETWORK_HELPER_HPP

#include <array>

std::array<unsigned char, 4> int_to_bytes(int n) {
    std::array<unsigned char, 4> bytes{};

    bytes[0] = (n >> 24) & 0xFF;
    bytes[1] = (n >> 16) & 0xFF;
    bytes[2] = (n >> 8) & 0xFF;
    bytes[3] = n & 0xFF;

    return bytes;
}

int bytes_to_int(std::array<unsigned char, 4> buffer) {
    return int((unsigned char) (buffer[0]) << 24 |
               (unsigned char) (buffer[1]) << 16 |
               (unsigned char) (buffer[2]) << 8 |
               (unsigned char) (buffer[3]));
}

#endif//NETWORKING_DEMO_NETWORK_HELPER_HPP
