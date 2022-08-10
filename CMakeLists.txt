cmake_minimum_required(VERSION 3.15)
project(networking_demo)

set(CMAKE_CXX_STANDARD 17)

if (MSVC)
    add_compile_definitions("_WIN32_WINNT=0x0601")
endif()

if (NOT EXISTS "${CMAKE_BINARY_DIR}/conan.cmake")
    message(STATUS "Downloading conan.cmake from https://github.com/conan-io/cmake-conan")
    file(DOWNLOAD "https://raw.githubusercontent.com/conan-io/cmake-conan/0.18.1/conan.cmake"
            "${CMAKE_BINARY_DIR}/conan.cmake"
            TLS_VERIFY ON)
endif ()

include(${CMAKE_BINARY_DIR}/conan.cmake)

conan_cmake_run(
        REQUIRES
        boost/1.71.0
        thrift/0.16.0
        zlib/1.2.12 # conflict resolution
        GENERATORS cmake
        BUILD missing
        BASIC_SETUP CMAKE_TARGETS
)

add_subdirectory(tcp)
add_subdirectory(udp)
add_subdirectory(thrift)
