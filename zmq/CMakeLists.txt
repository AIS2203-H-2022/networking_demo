
add_executable(zmq_server zmq_server.cpp)
target_link_libraries(zmq_server PUBLIC CONAN_PKG::zmqpp)

add_executable(zmq_client zmq_client.cpp)
target_link_libraries(zmq_client PUBLIC CONAN_PKG::zmqpp)
