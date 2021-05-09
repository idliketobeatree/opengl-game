#include "GameCamera.hpp"

#include <engine/rendering/rendering.hpp>
#include <settings.hpp>
#include <engine/logging/logging.hpp>

GameCamera::GameCamera(): Camera(), lastX(-5000), lastY(-5000), lastWindowWidth(-100), lastWindowHeight(-100) {}

void GameCamera::processInput(double dt) {
    float cameraSpeed = 5.0f * dt; // adjust accordingly
    bool updateView, updateProjection;

    if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS)
        cameraSpeed *= 2;

    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
        position += forward * cameraSpeed;
        updateView = true;
    }
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
        position -= forward * cameraSpeed;
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

    double mouseX, mouseY;
    glfwGetCursorPos(window, &mouseX, &mouseY);

    double xOffset = mouseX - lastX;
    double yOffset = mouseY - lastY;

    if(xOffset && yOffset) {
        yaw += xOffset * settings::sensitivity;
        pitch -= yOffset * settings::sensitivity;

        if (pitch > TAU / 4)
            pitch = TAU / 4;
        if (pitch < -TAU / 4)
            pitch = -TAU / 4;

        forward.x = cos(yaw) * cos(pitch);
        forward.y = sin(pitch);
        forward.z = sin(yaw) * cos(pitch);

        lastX = mouseX;
        lastY = mouseY;

        updateView = true;
    }
    int windowWidth, windowHeight;
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
    GLfloat cosPitch = cos(pitch);
    GLfloat sinPitch = sin(pitch);
    GLfloat cosYaw   = cos(yaw);
    GLfloat sinYaw   = sin(yaw);

    /// local X
    right = {
            -sinYaw,
            cosYaw,
            0
    };
    /// local Y
    up = {
            -sinPitch*cosYaw,
            -sinPitch*sinYaw,
            cosPitch
    };
    /// local Z
    forward = {
            cosPitch * cosYaw,
            cosPitch * sinYaw,
            sinPitch
    };
    debug("%f, %f, %f, %f", cosPitch, sinPitch, cosYaw, sinYaw);
    debug("%f, %f, %f", forward.x, forward.y, forward.z);
    debug("%f, %f", pitch, yaw);
    view = {
            right.x,    right.y,    right.z, 0,
            up.x,       up.y,       up.z, 0,
            forward.x,  forward.y,  forward.z, 0,
            position.x, position.y, position.z, 1
    };
}
void GameCamera::updateProjection() {

}