#include <opencv2/opencv.hpp>
#include <chrono>
#include "CameraProvider.hpp"
#include "KeyProcessor.hpp"
#include "FrameProcessor.hpp"
#include "Display.hpp"
#include "FaceDetector.hpp"

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

        cv::Rect rect(
            cv::Point(
                std::min(startPt.x, x),
                std::min(startPt.y, y)
            ),
            cv::Point(
                std::max(startPt.x, x),
                std::max(startPt.y, y)
            )
        );

        procPtr->addRectangle(rect);
    }
}

int main() {
    CameraProvider camera;
    KeyProcessor keyProc;
    FrameProcessor processor;
    Display display;
    FaceDetector detector(
    "models/deploy.prototxt",
    "models/res10_300x300_ssd_iter_140000.caffemodel"
    );

    procPtr = &processor;

    cv::namedWindow("Smart Vision");
    cv::setMouseCallback("Smart Vision", mouseCallback);

    int brightness = 50;
    cv::createTrackbar("Brightness", "Smart Vision", &brightness, 100);

    int fps = 0, frames = 0;
    auto start = std::chrono::high_resolution_clock::now();

    
    bool faceDetectionEnabled = false; 

    while (true) {
        if (cv::getWindowProperty("Smart Vision", cv::WND_PROP_VISIBLE) < 1) {
            break;
        }

        cv::Mat frame = camera.getFrame();
        if (frame.empty()) break;

        processor.setBrightness(brightness);
        Mode currentMode = keyProc.getMode();

        
        cv::Mat output = processor.process(frame, currentMode, fps);

        
        if (faceDetectionEnabled) {
            detector.updateFrame(frame); 
            auto faces = detector.getDetections();
            processor.drawFaces(output, faces); 
        }

        display.show(output);

        int key = cv::waitKey(1);
        if (key == 27) break; 

        // Керування фільтрами та трансформаціями
        if (key == 'w') processor.updateZoom(0.1);
        if (key == 's') processor.updateZoom(-0.1);
        if (key == 'a') processor.updateRotation(5);
        if (key == 'd') processor.updateRotation(-5);
        if (key == 'r') processor.clearRectangles();

        // ОБРОБКА КЛАВІШІ F ЯК ГЛОБАЛЬНОГО ОВЕРЛЕЮ
        if (key == 'f' || key == 'F') {
            faceDetectionEnabled = !faceDetectionEnabled;
            if (faceDetectionEnabled) {
                detector.start();
            } else {
                detector.stop();
            }
        }

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

    detector.stop();
    cv::destroyAllWindows();
    return 0;
    
}