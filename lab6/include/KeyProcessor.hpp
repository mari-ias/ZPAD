#pragma once

enum class Mode {
    NORMAL, GRAYSCALE, CANNY, BLUR, INVERT,
    SOBEL, GLITCH, PIP
};

class KeyProcessor {
private:
    Mode currentMode = Mode::NORMAL;

public:
    void processKey(int key);
    Mode getMode() const { return currentMode; }
};