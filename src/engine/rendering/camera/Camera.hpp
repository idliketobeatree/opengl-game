#pragma once

#include <math/math.hpp>
#include <engine/rendering/typedefs.hpp>

class Camera {
public:
    Matrix4f view, projection;

    Vector3f position;

    Vector3f direction, right, up;

    /// TODO: quaternions or whatever I guess just make it better lol
    GLdouble pitch, yaw, roll;

    Camera();
    Camera(Vector3f position, GLdouble pitch, GLdouble yaw, GLdouble roll);

    void setup();

    void processInput(double dt);

    void updateView();
    void updateProjection();
};