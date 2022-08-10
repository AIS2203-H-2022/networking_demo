
add_executable(tcp_server tcp_server.cpp)
target_link_libraries(tcp_server PUBLIC CONAN_PKG::boost)

add_executable(tcp_client tcp_client.cpp)
target_link_libraries(tcp_client PUBLIC CONAN_PKG::boost)
