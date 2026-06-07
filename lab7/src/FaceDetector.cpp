#include "FaceDetector.hpp"
#include <iostream>
#include <algorithm>
#include <chrono>

// Конструктор: приймає шляхи до файлів, які передаються з main.cpp
FaceDetector::FaceDetector(const std::string& prototypePath, const std::string& modelPath) {
    net = cv::dnn::readNetFromCaffe(prototypePath, modelPath);
    if (net.empty()) {
        std::cerr << "Error: Could not load Caffe neural network!" << std::endl;
    } else {
        std::cout << "Neural network loaded successfully from " << modelPath << std::endl;
    }
}

// Деструктор: автоматично зупиняє потік при видаленні об'єкта
FaceDetector::~FaceDetector() {
    stop();
}

// Запуск фонового потоку
void FaceDetector::start() {
    if (!running) {
        running = true;
        worker = std::thread(&FaceDetector::run, this);
        std::cout << "Background FaceDetector thread started." << std::endl;
    }
}

// Безпечна зупинка потоку
void FaceDetector::stop() {
    if (running) {
        running = false; // Опускаємо прапорець атоміка
        if (worker.joinable()) {
            worker.join(); // Чекаємо завершення потоку
        }
        std::cout << "Background FaceDetector thread stopped." << std::endl;
    }
}

// Передача нового кадру з основного потоку (під м'ютексом)
void FaceDetector::updateFrame(const cv::Mat& frame) {
    std::lock_guard<std::mutex> lock(mtx_frame);
    currentFrame = frame.clone(); // Робимо копію матриці, щоб уникнути race condition
}

// Забирання результатів детекції в основний потік (під м'ютексом)
std::vector<cv::Rect> FaceDetector::getDetections() {
    std::lock_guard<std::mutex> lock(mtx_res);
    return detections;
}

// Фоновий цикл роботи нейромережі
void FaceDetector::run() {
    while (running) {
        cv::Mat img;
        
        // Безпечно копіюємо кадр для обробки
        {
            std::lock_guard<std::mutex> lock(mtx_frame);
            if (currentFrame.empty()) {
                std::this_thread::sleep_for(std::chrono::milliseconds(10));
                continue; 
            }
            img = currentFrame.clone();
        }

        // 1. Конвертуємо зображення у Blob (формат для нейромереж)
        // Розмір 300x300 та середні кольори (104, 177, 123) прописані в специфікації ResNet SSD
        cv::Mat blob = cv::dnn::blobFromImage(img, 1.0, cv::Size(300, 300), 
                                              cv::Scalar(104.0, 177.0, 123.0), false, false);
        
        net.setInput(blob);
        
        // 2. Прямий прохід (інференс)
        cv::Mat output = net.forward();
        
        // Результат мережі має формат 4D масиву, трансформуємо його у зручну матрицю
        cv::Mat detectionMat(output.size[2], output.size[3], CV_32F, output.ptr<float>());

        std::vector<cv::Rect> foundFaces;

        // 3. Проходимо по всіх знайдених об'єктах
        for (int i = 0; i < detectionMat.rows; i++) {
            float confidence = detectionMat.at<float>(i, 2); // Ступінь впевненості (0.0 - 1.0)

            // Фільтруємо обличчя з впевненістю > 50%
            if (confidence > 0.5f) {
                // Нейромережа повертає нормалізовані координати (від 0 до 1),
                // тому множимо їх на реальну ширину та висоту нашого кадру
                int x1 = static_cast<int>(detectionMat.at<float>(i, 3) * img.cols);
                int y1 = static_cast<int>(detectionMat.at<float>(i, 4) * img.rows);
                int x2 = static_cast<int>(detectionMat.at<float>(i, 5) * img.cols);
                int y2 = static_cast<int>(detectionMat.at<float>(i, 6) * img.rows);

                // Перевіряємо, щоб координати не виходили за межі кадру
                x1 = std::max(0, x1);
                y1 = std::max(0, y1);
                x2 = std::min(img.cols - 1, x2);
                y2 = std::min(img.rows - 1, y2);

                // Додаємо прямокутник у тимчасовий вектор
                foundFaces.push_back(cv::Rect(cv::Point(x1, y1), cv::Point(x2, y2)));
            }
        }

        // 4. Записуємо знайдені координати у спільну змінну під м'ютексом
        {
            std::lock_guard<std::mutex> lock(mtx_res);
            detections = foundFaces;
        }     

    }
}