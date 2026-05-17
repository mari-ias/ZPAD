#pragma once

#include <opencv2/opencv.hpp>
#include <opencv2/dnn.hpp>

#include <thread>
#include <mutex>
#include <atomic>
#include <vector>
#include <string>

class FaceDetector {

public:
    FaceDetector(
        const std::string& prototypePath,
        const std::string& modelPath
    );

    ~FaceDetector();

    void updateFrame(const cv::Mat& frame);

    std::vector<cv::Rect> getDetections();

    void start();

    void stop();

private:
    void run();

    cv::dnn::Net net;

    std::thread worker;

    std::mutex mtx_frame;
    std::mutex mtx_res;

    std::atomic<bool> running{false};

    cv::Mat currentFrame;

    std::vector<cv::Rect> detections;
};

