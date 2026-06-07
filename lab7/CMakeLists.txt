cmake_minimum_required(VERSION 3.10)

project(Lab7_OpenCV)

set(CMAKE_CXX_STANDARD 17)

find_package(OpenCV REQUIRED)

add_executable(lab7
    src/main.cpp
    src/CameraProvider.cpp
    src/FrameProcessor.cpp
    src/KeyProcessor.cpp
    src/Display.cpp
    src/FaceDetector.cpp
)

target_include_directories(lab7 PRIVATE include)

target_link_libraries(lab7
    ${OpenCV_LIBS}
    pthread
)