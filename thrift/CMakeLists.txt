
set(thriftPythonSourcesDir "${CMAKE_SOURCE_DIR}/thrift/python")
set(thriftCppSourcesDir "${generatedSourcesDir}/thrift/cpp/demo/thrift")

file(MAKE_DIRECTORY "${thriftCppSourcesDir}")
file(MAKE_DIRECTORY "${thriftPythonSourcesDir}")

message(${thriftPythonSourcesDir})

find_program(THRIFT_EXECUTABLE NAMES thrift HINTS "${CONAN_THRIFT_ROOT}/bin")

if (NOT THRIFT_EXECUTABLE)
    message(FATAL_ERROR "The thrift compiler was not found. Cannot generate Thrift sources.")
endif ()

# generate cpp sources
execute_process(
        COMMAND "${THRIFT_EXECUTABLE}"
        "--gen" "cpp:no_skeleton"
        "-out" "${thriftCppSourcesDir}"
        "${CMAKE_CURRENT_SOURCE_DIR}/service.thrift"
)

# generate py sources
execute_process(
        COMMAND "${THRIFT_EXECUTABLE}"
        "--gen" "py"
        "-out" "${thriftPythonSourcesDir}"
        "${CMAKE_CURRENT_SOURCE_DIR}/service.thrift"
)

set(thriftGenerated
        "${thriftCppSourcesDir}/ThriftDemo.cpp"
        "${thriftCppSourcesDir}/ThriftDemo.h"
        )

add_library(thrift_service ${thriftGenerated})
target_include_directories(thrift_service PUBLIC ${generatedSourcesDir}/thrift/cpp)
target_link_libraries(thrift_service PUBLIC CONAN_PKG::thrift)

add_executable(thrift_server thrift_server.cpp)
target_link_libraries(thrift_server PUBLIC thrift_service)

add_executable(thrift_client thrift_client.cpp)
target_link_libraries(thrift_client PUBLIC thrift_service)
