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
    float cameraSpeed = 50.f * dt; // adjust accordingly
    bool updateView = false, updateProjection = false;

    if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS)
        cameraSpeed *= 2;

    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
        position +=  Vector3f{1, 0,0} * cameraSpeed;
        updateView = true;
    }
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
        position -= Vector3f{1, 0, 0} * cameraSpeed;
        updateView = true;
    }
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
        position -= Vector3f{0, 0, 1} * cameraSpeed;
        updateView = true;
    }
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
        position += Vector3f{0, 0,1} * cameraSpeed;
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

        direction.x = cos(yaw) * cos(pitch);
        direction.y = sin(pitch);
        direction.z = sin(yaw) * cos(pitch);

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
    Vector3<GLfloat> dir = Vector3<GLfloat>::normalize(direction);    // The "forward" vector.
    Vector3<GLfloat> right = Vector3<GLfloat>::normalize(Vector3<GLfloat>::cross(Vector3<GLfloat>{0.f,1.f,0.f}, dir));// The "right" vector.
    Vector3<GLfloat> up = Vector3<GLfloat>::cross(dir, right);

    //this doesn't look column major but thats just how the GPU 'interprets the array'.
    view = {
            right.x,  up.x, dir.x, 0,
            right.y,  up.y, dir.y, 0,
            right.z,  up.z, dir.z, 0,
            -right.dot(position), -up.dot(position), -dir.dot(position), 1
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