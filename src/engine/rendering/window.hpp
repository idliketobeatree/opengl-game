#pragma once

#include <GLFW/glfw3.h>
#include <printf.h>

struct Window {
    GLFWwindow *ptr;

    const char *title;

    int xSize;
    int ySize;

    Window(const int xSize, const int ySize, const char *title) : xSize(xSize), ySize(ySize), title(title) {}

    bool create() {
        ptr = glfwCreateWindow(xSize, ySize, title, NULL, NULL);
        return ptr;
    }

    operator GLFWwindow*() const {
        return ptr;
    }
};