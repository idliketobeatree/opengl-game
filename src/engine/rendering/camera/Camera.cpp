#include "Camera.hpp"

Camera::Camera(): position{0, 0, 0}, yaw(0), pitch(0), roll(0) {}
Camera::Camera(Vector3<GLfloat> position, GLdouble pitch, GLdouble yaw, GLdouble roll): position(position), yaw(yaw), pitch(pitch), roll(roll) {}