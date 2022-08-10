set(ddsCppSourcesDir "${generatedSourcesDir}/dds/cpp")

file(MAKE_DIRECTORY "${ddsCppSourcesDir}")

execute_process(COMMAND "java"
        "-jar" "${CMAKE_CURRENT_SOURCE_DIR}/fastddsgen.jar" "${CMAKE_CURRENT_SOURCE_DIR}/HelloWorld.idl"
        "-d" "${ddsCppSourcesDir}")

set(ddsGenerated
        "${ddsCppSourcesDir}/HelloWorld.cxx"
        "${ddsCppSourcesDir}/HelloWorld.h"
        "${ddsCppSourcesDir}/HelloWorldPubSubTypes.cxx"
        "${ddsCppSourcesDir}/HelloWorldPubSubTypes.h"
        )

add_library(dds_service ${ddsGenerated})
target_include_directories(dds_service PUBLIC ${ddsCppSourcesDir})
target_link_libraries(dds_service PUBLIC CONAN_PKG::fast-dds)

add_executable(HelloWorldPublisher HelloWorldPublisher.cpp)
target_link_libraries(HelloWorldPublisher PUBLIC dds_service)

add_executable(HelloWorldSubscriber HelloWorldSubscriber.cpp)
target_link_libraries(HelloWorldSubscriber PUBLIC dds_service)
