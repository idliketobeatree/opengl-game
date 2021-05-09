#pragma once

#include <engine/math/math.hpp>
#include <engine/rendering/typedefs.hpp>

class Camera {
public:
    Matrix4<GLdouble> view, projection;

    Vector3<GLfloat> position;
    Vector3<GLfloat> forward, up, right;

    /// TODO: quaternions or whatever I guess just make it better lol
    GLdouble pitch, yaw, roll;

    Camera();
    Camera(Vector3<GLfloat> position, GLdouble pitch, GLdouble yaw, GLdouble roll);

    void processInput(double dt);

    void updateView();
    void updateProjection();
};