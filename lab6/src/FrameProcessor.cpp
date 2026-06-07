#include "FrameProcessor.hpp"
#include <iostream>

FrameProcessor::FrameProcessor() {
    logo = cv::imread("../assets/logo.png", cv::IMREAD_COLOR);

    if (logo.empty()) {
        std::cerr << "Warning: logo not loaded\n";
    }
}

void FrameProcessor::setBrightness(int val) {
    brightness = val;
}

void FrameProcessor::updateZoom(double delta) {
    zoomFactor += delta;
    if (zoomFactor < 0.1) zoomFactor = 0.1;
}

void FrameProcessor::updateRotation(double delta) {
    rotationAngle += delta;
}

void FrameProcessor::addRectangle(const cv::Rect& rect) {
    rectangles.push_back(rect);
}

void FrameProcessor::clearRectangles() {
    rectangles.clear();
}

cv::Mat FrameProcessor::process(const cv::Mat& frame, Mode mode, int fps) {
    cv::Mat result = frame.clone();


    // IMAGE PROCESSING MODES
    switch (mode) {
        case Mode::GRAYSCALE:
            cv::cvtColor(result, result, cv::COLOR_BGR2GRAY);
            cv::cvtColor(result, result, cv::COLOR_GRAY2BGR);
            break;

        case Mode::CANNY: {
            cv::Mat gray;
            cv::cvtColor(result, gray, cv::COLOR_BGR2GRAY);
            cv::Canny(gray, result, 100, 200);
            cv::cvtColor(result, result, cv::COLOR_GRAY2BGR);
            break;
        }

        case Mode::BLUR:
            cv::GaussianBlur(result, result, cv::Size(15, 15), 0);
            break;

        case Mode::INVERT:
            cv::bitwise_not(result, result);
            break;

        case Mode::SOBEL: {
            cv::Mat gray, sobel;
            cv::cvtColor(result, gray, cv::COLOR_BGR2GRAY);
            cv::Sobel(gray, sobel, CV_8U, 1, 1);
            cv::cvtColor(sobel, result, cv::COLOR_GRAY2BGR);
            break;
        }

        case Mode::GLITCH: {
            std::vector<cv::Mat> channels;
            cv::split(result, channels);

            int shift = 10;
            if (result.cols > shift) {
                cv::Mat shifted = cv::Mat::zeros(channels[2].size(), channels[2].type());

                channels[2](cv::Rect(shift, 0, channels[2].cols - shift, channels[2].rows))
                    .copyTo(shifted(cv::Rect(0, 0, channels[2].cols - shift, channels[2].rows)));

                channels[2] = shifted;
                cv::merge(channels, result);
            }
            break;
        }

        case Mode::PIP:
            if (!logo.empty()) {
                cv::Mat resized;

                int w = std::min(120, result.cols - 10);
                int h = std::min(120, result.rows - 10);

                if (w > 0 && h > 0) {
                    cv::resize(logo, resized, cv::Size(w, h));
                    resized.copyTo(result(cv::Rect(10, 10, w, h)));
                }
            }
            break;

        default:
            break;
    }


    // BRIGHTNESS CONTROL
    double beta = std::clamp(brightness - 50, -50, 50);
    result.convertTo(result, -1, 1, beta);


    // ROTATION + ZOOM
    cv::Mat transform = cv::getRotationMatrix2D(
        cv::Point2f(result.cols / 2.0f, result.rows / 2.0f),
        rotationAngle,
        zoomFactor
    );

    cv::warpAffine(result, result, transform, result.size());


    // DRAW RECTANGLES
    for (const auto& rect : rectangles) {
        cv::rectangle(result, rect, cv::Scalar(0, 255, 0), 2);
    }


    // FPS TEXT OVERLAY
    cv::putText(result,
        "FPS: " + std::to_string(fps),
        cv::Point(10, 30),
        cv::FONT_HERSHEY_SIMPLEX,
        0.8,
        cv::Scalar(0, 255, 255),
        2,
        cv::LINE_AA
    );

    return result;
}
