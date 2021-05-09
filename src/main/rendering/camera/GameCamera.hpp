#pragma once

#include <engine/rendering/camera/Camera.hpp>

class GameCamera: public Camera {
protected:
    double mouseX, mouseY, lastX, lastY;
    int windowWidth, windowHeight, lastWindowWidth, lastWindowHeight;
public:
    GameCamera();

    void setup();

    void processInput(double dt);

    void updateView();
    void updateProjection();
};