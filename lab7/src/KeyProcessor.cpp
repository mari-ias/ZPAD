#include "KeyProcessor.hpp"

void KeyProcessor::processKey(int key) {
    if (key == -1) return;

    switch (key) {
        case '0': currentMode = Mode::NORMAL; break;
        case '1': currentMode = Mode::GRAYSCALE; break;
        case '2': currentMode = Mode::CANNY; break;
        case '3': currentMode = Mode::BLUR; break;
        case '4': currentMode = Mode::INVERT; break;
        case '5': currentMode = Mode::SOBEL; break;
        case '6': currentMode = Mode::GLITCH; break;
        case '7': currentMode = Mode::PIP; break;
        // case 'f': case 'F': currentMode = Mode::FACE; break;
        default:
            break;
    }
}

