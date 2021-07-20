#include <rendering/rendering.hpp>
#include <logging/logging.hpp>
#include <rendering/renderer/DebugRenderer.hpp>
#include <rendering/renderer/ChunkRenderer.hpp>

Window window(1680, 950, "OpenGL Window");
GameRenderer gameRenderer;
DebugRenderer debugRenderer;
GameCamera camera;

void framebufferSizeCallback(GLFWwindow *window, int width, int height) {
    glfwMakeContextCurrent(window);
    glViewport(0, 0, width, height);
}

int main() {
    changeLevel(-127);
    info("My custom OpenGL engine/game :)");
    info("Github/MeowyAsh | Unlicense");
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

    /// GLAD init

    if (!gladLoadGLLoader((GLADloadproc) glfwGetProcAddress)) {
        fatal("Failed to initialize GLAD :(");
        return -1;
    }

    /// Info
    straightSeparator();
    info("OpenGL version: %s", glGetString(GL_VERSION));
    info("GLFW version:   %s", glfwGetVersionString());
    straightSeparator();
    line();

    info("%d", Chunk::getIndex({CHUNK_SIZE, CHUNK_SIZE, CHUNK_SIZE}));

    /// get rendering set up
    gameRenderer.setupRender();
    debugRenderer.setupRender();
    camera.setup();

    /// Main loop

    double time;
    double deltaTime;
    while (!glfwWindowShouldClose(window)) {

        camera.processInput(deltaTime);
        gameRenderer.render(deltaTime);
        debugRenderer.render(deltaTime);

        frame++;

        time = glfwGetTime();
        glfwSwapBuffers(window);
        glfwPollEvents();
        deltaTime = glfwGetTime() - time;
    }

    glfwTerminate();

}