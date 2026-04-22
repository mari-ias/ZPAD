#include "CameraProvider.hpp"
#include <iostream>

CameraProvider::CameraProvider() {
    // Відкриваємо індекс 0 з явним використанням драйвера V4L2
    cap.open(0, cv::CAP_V4L2);

    if (cap.isOpened()) {
        // Налаштовуємо формат MJPEG - він значно легший для передачі через usbipd
        cap.set(cv::CAP_PROP_FOURCC, cv::VideoWriter::fourcc('M', 'J', 'P', 'G'));
        
        // Встановлюємо стандартну роздільну здатність
        cap.set(cv::CAP_PROP_FRAME_WIDTH, 640);
        cap.set(cv::CAP_PROP_FRAME_HEIGHT, 480);
        
        std::cout << "Camera initialized successfully on /dev/video0" << std::endl;
    } else {
        std::cerr << "Cannot open camera on index 0\n";
    }
}

CameraProvider::~CameraProvider() {
    if (cap.isOpened()) {
        cap.release();
    }
}

cv::Mat CameraProvider::getFrame() {
    cv::Mat frame;
    cap >> frame;
    if (frame.empty()) {
        std::cerr << "Warning: empty frame captured\n";
    }

    return frame;
}