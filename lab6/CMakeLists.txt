cmake_minimum_required(VERSION 3.10)
project(Lab6_OpenCV)

set(CMAKE_CXX_STANDARD 17)

find_package(OpenCV REQUIRED)

add_executable(lab6
    src/main.cpp
    src/CameraProvider.cpp
    src/FrameProcessor.cpp
    src/KeyProcessor.cpp
    src/Display.cpp
)

target_include_directories(lab6 PRIVATE include)

target_link_libraries(lab6 ${OpenCV_LIBS})