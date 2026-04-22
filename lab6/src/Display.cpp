#include "Display.hpp"

void Display::show(const cv::Mat& frame) {
    cv::imshow("Smart Vision", frame);
}