#pragma once
#include <opencv2/opencv.hpp>
#include <vector>
#include "KeyProcessor.hpp"

class FrameProcessor {
private:
    int brightness = 50;
    double zoomFactor = 1.0;
    double rotationAngle = 0.0;

    cv::Mat logo;
    std::vector<cv::Rect> rectangles;

public:
    FrameProcessor();

    void setBrightness(int val);
    void updateZoom(double delta);
    void updateRotation(double delta);

    void addRectangle(const cv::Rect& rect);
    void clearRectangles();

    cv::Mat process(const cv::Mat& frame, Mode mode, int fps);
};