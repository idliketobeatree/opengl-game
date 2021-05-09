#pragma once

#include <engine/rendering/camera/Camera.hpp>

class GameCamera: public Camera {
protected:
    double lastX, lastY;
    int lastWindowWidth, lastWindowHeight;
public:
    GameCamera();

    void processInput(double dt);

    void updateView();
    void updateProjection();
};