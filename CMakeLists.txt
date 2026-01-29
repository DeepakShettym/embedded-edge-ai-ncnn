cmake_minimum_required(VERSION 3.18)
project(ai_inference_service)

set(CMAKE_CXX_STANDARD 17)

set(NCNN_INSTALL_DIR $ENV{HOME}/ncnn/build-android/install)

include_directories(${NCNN_INSTALL_DIR}/include)

add_executable(ai_inference_service
    src/main.cpp
)

add_library(ncnn SHARED IMPORTED)
set_target_properties(ncnn PROPERTIES
    IMPORTED_LOCATION ${NCNN_INSTALL_DIR}/lib/libncnn.so
)

target_link_libraries(ai_inference_service
    ncnn
    android
    log
)

add_executable(ai_image_infer
    src/image_infer.cpp
)

target_link_libraries(ai_image_infer
    ncnn
    android
    log
)

add_executable(thermal_ai_service
    src/thermal_ai_service.cpp
)

target_link_libraries(thermal_ai_service
    ncnn
    android
    log
)

add_executable(system_ai_governor src/system_ai_governor.cpp)
target_link_libraries(system_ai_governor ncnn)
