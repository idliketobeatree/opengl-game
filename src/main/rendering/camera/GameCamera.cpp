#include "GameCamera.hpp"

#include <stdlib.h>

#include <engine/rendering/rendering.hpp>
#include <settings.hpp>
#include <logging/logging.hpp>

GameCamera::GameCamera(): Camera(Vector3f{0,0,3}, 0, 0, 0) {
}

void GameCamera::setup() {
    glfwGetWindowSize(window, &windowWidth, &windowHeight);
    this->updateProjection();
    this->lastWindowWidth  = windowWidth;
    this->lastWindowHeight = windowHeight;
    glfwGetCursorPos(window, &mouseX, &mouseY);
    this->updateView();
    this->lastX = mouseX;
    this->lastY = mouseY;
}

void GameCamera::processInput(double dt) {
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    float cameraSpeed = 20.0f * dt; // adjust accordingly
    bool updateView = false, updateProjection = false;

    if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS)
        cameraSpeed *= 2;

    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
        position -=  direction * cameraSpeed;
        updateView = true;
    }
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
        position += direction * cameraSpeed;
        updateView = true;
    }
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
        position -= right * cameraSpeed;
        updateView = true;
    }
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
        position += right * cameraSpeed;
        updateView = true;
    }
    if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS) {
        position += up * cameraSpeed;
        updateView = true;
    }
    if (glfwGetKey(window, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS) {
        position -= up * cameraSpeed;
        updateView = true;
    }
    if (glfwGetKey(window, GLFW_KEY_DELETE) == GLFW_PRESS) {
        exit(1);
    }

    glfwGetCursorPos(window, &mouseX, &mouseY);

    double xOffset = mouseX - lastX;
    double yOffset = mouseY - lastY;

    if(xOffset || yOffset) {
        yaw   += xOffset * settings::sensitivity * dt;
        pitch += yOffset * settings::sensitivity * dt;

        if (pitch > TAU / 4)
            pitch = TAU / 4;
        if (pitch < -TAU / 4)
            pitch = -TAU / 4;

        lastX = mouseX;
        lastY = mouseY;

        updateView = true;
    }
    glfwGetWindowSize(window, &windowWidth, &windowHeight);
    if (windowWidth != lastWindowWidth || windowHeight != lastWindowHeight) {
        updateProjection = true;
        lastWindowWidth = windowWidth;
        lastWindowHeight = windowHeight;
    }

    if(updateView)
        this->updateView();
    if(updateProjection)
        this->updateProjection();
}

void GameCamera::updateView() {
    direction = {(float)cos(yaw) * (float)cos(pitch), (float)sin(pitch), (float)sin(yaw) * (float)cos(pitch)};
    right = Vector3f::normalize(Vector3f::cross(Vector3f{0.f,1.f,0.f}, direction));// The "right" vector.
    up = Vector3f::cross(direction, right);

    //this doesn't look column major but that's just how the GPU 'interprets the array'.
    view = {
            right.x,  up.x, direction.x, 0,
            right.y,  up.y, direction.y, 0,
            right.z,  up.z, direction.z, 0,
            -right.dot(position), -up.dot(position), -direction.dot(position), 1
    };
}
void GameCamera::updateProjection() {
    const GLfloat tanHalfFovy = tan(settings::fov / 2);
    const GLfloat aspect = (float)windowWidth / windowHeight;

    projection[0] = 1 / (aspect * tanHalfFovy);
    projection[5] = 1 / tanHalfFovy;
    projection[10] = - (settings::far + settings::near) / (settings::far - settings::near);
    projection[11] = -1;
    projection[14] = -(2 * settings::far * settings::near) / (settings::far - settings::near);
}