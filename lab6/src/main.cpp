#include <opencv2/opencv.hpp>
#include <chrono>
#include "CameraProvider.hpp"
#include "KeyProcessor.hpp"
#include "FrameProcessor.hpp"
#include "Display.hpp"

cv::Point startPt;
bool drawing = false;
FrameProcessor* procPtr;

void mouseCallback(int event, int x, int y, int, void*) {
    if (event == cv::EVENT_LBUTTONDOWN) {
        drawing = true;
        startPt = {x,y};
    }
    else if (event == cv::EVENT_LBUTTONUP) {
        drawing = false;
        procPtr->addRectangle(cv::Rect(startPt, cv::Point(x,y)));
    }
}

int main() {
    CameraProvider camera;
    KeyProcessor keyProc;
    FrameProcessor processor;
    Display display;

    procPtr = &processor;

    cv::namedWindow("Smart Vision");
    cv::setMouseCallback("Smart Vision", mouseCallback);

    int brightness = 50;
    cv::createTrackbar("Brightness", "Smart Vision", &brightness, 100);

    int fps = 0, frames = 0;
    auto start = std::chrono::high_resolution_clock::now();

    while (true) {
        if (cv::getWindowProperty("Smart Vision", cv::WND_PROP_VISIBLE) < 1) {
            break;
        }
        
        cv::Mat frame = camera.getFrame();
        if (frame.empty()) break;

        processor.setBrightness(brightness);

        cv::Mat output = processor.process(frame, keyProc.getMode(), fps);

        display.show(output);

        int key = cv::waitKey(1);
        if (key == 27) break;

        // control
        if (key == 'w') processor.updateZoom(0.1);
        if (key == 's') processor.updateZoom(-0.1);
        if (key == 'a') processor.updateRotation(5);
        if (key == 'd') processor.updateRotation(-5);
        if (key == 'r') processor.clearRectangles();

        keyProc.processKey(key);

        frames++;
        auto now = std::chrono::high_resolution_clock::now();
        double sec = std::chrono::duration<double>(now - start).count();
        if (sec >= 1.0) {
            fps = frames;
            frames = 0;
            start = now;
        }
    }

    return 0;
}