#include <rendering/rendering.hpp>
#include <engine/logging/logging.hpp>

#include <engine/math/math.hpp>

Window window(640, 480, "OpenGL Window");
GameRenderer gameRenderer;
GameCamera camera;

void framebufferSizeCallback(GLFWwindow *window, int width, int height) {
    glfwMakeContextCurrent(window);
    glViewport(0, 0, width, height);
}

int main() {
    changeLevel(-127);
    info("My custom OpenGL engine/game :)");
    info("Github/Meowy1227 | Unlicense");
    line();

    /// GLFW stuff

    if (!glfwInit())
        return -1;

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // Make MacOS shut up

    if (!window.create()) {
        fatal("Failed to create GLFW window :(");
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(window);
    glfwSwapInterval(1);

    glfwSetFramebufferSizeCallback(window, framebufferSizeCallback);

    /// GLEW init

//    glewExperimental = true; // Needed for core profile
//    if (glewInit() != GLEW_OK) {
//        fatal("Failed to initialize GLEW :(");
//        glfwTerminate();
//        return -1;
//    }

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        fatal("Failed to initialize GLAD :(");
        return -1;
    }

    /// Info
    straightSeparator();
    info("OpenGL version: %s", glGetString(GL_VERSION));
    info("GLFW version:   %s", glfwGetVersionString());
//    info("GLEW version:   %s", glewGetString(GLEW_VERSION));
//    info("GLAD version:   %s", GLAD_VERSION);
    straightSeparator();
    line();

    /// Game renderer setup
    gameRenderer.setupRender();

    /// Main loop

    double time;
    double deltaTime;
    while (!glfwWindowShouldClose(window)) {
        time = glfwGetTime();

        camera.processInput(deltaTime);
        gameRenderer.render(deltaTime);

        frame++;
        deltaTime = glfwGetTime() - time;

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwTerminate();
}