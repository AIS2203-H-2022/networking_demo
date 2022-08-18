execute_process(COMMAND "java"
        "-jar" "${CMAKE_CURRENT_SOURCE_DIR}/../fastddsgen.jar" "${CMAKE_CURRENT_SOURCE_DIR}/Image.idl"
        "-d" "${ddsCppSourcesDir}")

set(ddsImageGenerated
        "${ddsCppSourcesDir}/Image.cxx"
        "${ddsCppSourcesDir}/Image.h"
        "${ddsCppSourcesDir}/ImagePubSubTypes.cxx"
        "${ddsCppSourcesDir}/ImagePubSubTypes.h"
        )

add_library(dds_image_service ${ddsImageGenerated})
target_include_directories(dds_image_service PUBLIC ${ddsCppSourcesDir})
target_link_libraries(dds_image_service PUBLIC CONAN_PKG::fast-dds)


add_executable(ImagePublisher ImagePublisher.cpp)
target_link_libraries(ImagePublisher PUBLIC dds_image_service CONAN_PKG::opencv)

add_executable(ImageSubscriber ImageSubscriber.cpp)
target_link_libraries(ImageSubscriber PUBLIC dds_image_service CONAN_PKG::opencv)
