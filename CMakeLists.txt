cmake_minimum_required(VERSION 3.15)
project(networking_demo)

option(WITH_THRIFT "Build Thrift demo" OFF)
option(WITH_DDS "Build DDS demo" OFF)
option(WITH_ZMQ "Build ZMQ demo" OFF)
option(WITH_ALL "Build all demos (overrides individual settings)" OFF)

set(CMAKE_CXX_STANDARD 17)
set(generatedSourcesDir ${CMAKE_BINARY_DIR}/generated)
file(MAKE_DIRECTORY "${generatedSourcesDir}")

if (MSVC)
    # Boost asio wants this...
    add_compile_definitions("_WIN32_WINNT=0x0601")
endif ()

include(cmake/conan.cmake)

set(DEPS boost/1.71.0)
if (WITH_THRIFT OR WITH_ALL)
    list(APPEND DEPS thrift/0.16.0 zlib/1.2.12)
endif ()
if (WITH_DDS OR WITH_ALL)
    list(APPEND DEPS fast-dds/2.3.4)
endif ()
if (WITH_ZMQ OR WITH_ALL)
    list(APPEND DEPS zmqpp/4.2.0)
endif ()

conan_cmake_run(
        REQUIRES
        ${DEPS}
        GENERATORS cmake
        BUILD missing
        BASIC_SETUP CMAKE_TARGETS
)

add_subdirectory(tcp)
add_subdirectory(udp)
if (WITH_THRIFT OR WITH_ALL)
    add_subdirectory(thrift)
endif ()
if (WITH_DDS OR WITH_ALL OR WITH_ALL)
    add_subdirectory(dds)
endif ()
if (WITH_ZMQ OR WITH_ALL)
    add_subdirectory(zmq)
endif ()
