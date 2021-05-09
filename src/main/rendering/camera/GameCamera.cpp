#include "GameCamera.hpp"

#include <engine/rendering/rendering.hpp>
#include <settings.hpp>
#include <engine/logging/logging.hpp>

GameCamera::GameCamera(): Camera(Vector3<GLfloat>{0,0,-3}, 0, 0, 0) {
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
//    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    float cameraSpeed = 5000.0f * dt; // adjust accordingly
    bool updateView, updateProjection;

    if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS)
        cameraSpeed *= 2;

    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
        position += Vector3f{forward.x, 0, forward.z} * cameraSpeed;
        updateView = true;
    }
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
        position -= Vector3f{forward.x, 0, forward.z} * cameraSpeed;
        updateView = true;
    }
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
        position -= Vector3f{right.x, 0, right.z} * cameraSpeed;
        updateView = true;
    }
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
        position += Vector3f{right.x, 0, right.z} * cameraSpeed;
        updateView = true;
    }
    if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS) {
        position += Vector3f{0, 1, 0} * cameraSpeed;
        updateView = true;
    }
    if (glfwGetKey(window, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS) {
        position -= Vector3f{0, 1, 0} * cameraSpeed;
        updateView = true;
    }

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
    debug("\nright:    %.1f, %.1f, %.1f, %.1f,\n"
          "up:       %.1f, %.1f, %.1f, %.1f,\n"
          "forward:  %.1f, %.1f, %.1f, %.1f,\n"
          "position: %.1f, %.1f, %.1f, %.1f",
          view[0],  view[1],  view[2],  view[3],
          view[4],  view[5],  view[6],  view[7],
          view[8],  view[9],  view[10], view[11],
          view[12], view[13], view[14], view[15]
          );
    view = {
            right.x,    right.y,    right.z, 0,
            up.x,       up.y,       up.z, 0,
            forward.x,  forward.y,  forward.z, 0,
            position.x, position.y, position.z, 1
    };
}
void GameCamera::updateProjection() {
    const GLfloat tanHalfFovy = tan(settings::fov / 2);
    const GLfloat aspect = windowWidth / windowHeight;

    projection[0] = 1 / (aspect * tanHalfFovy);
    projection[5] = 1 / tanHalfFovy;
    projection[10] = - (settings::far + settings::near) / (settings::far - settings::near);
    projection[14] = -1;
    projection[11] = -(2 * settings::far * settings::near) / (settings::far - settings::near);
}