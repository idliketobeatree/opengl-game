#include <rendering/rendering.hpp>
#include <logging/logging.hpp>
#include <rendering/renderer/DebugRenderer.hpp>
#include <rendering/renderer/ChunkRenderer.hpp>

#include <math/math.hpp>
#include <world/block/blocks.hpp>

#include <math/Noise.hpp>

Window window(1680, 950, "OpenGL Window");
GameRenderer gameRenderer;
DebugRenderer debugRenderer;
ChunkRenderer chunkRenderer(new Chunk({0, 0, 0}), 0.5f);
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

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
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
    Vector3u8 pos;
    Noise noise;
    for(pos.x = 0; pos.x < CHUNK_SIZE; pos.x++) {
        for(pos.y = 0; pos.y < CHUNK_SIZE; pos.y++) {
            for(pos.z = 0; pos.z < CHUNK_SIZE; pos.z++) {
//                float noiseV = noise.noise3f(pos.x/16.0f, pos.y/16.0f, pos.z/16.0f);
                float noiseV = noise.noise3f(pos.x/20.0f, pos.y/20.0f, pos.z/20.0f);
                if(noiseV > chunkRenderer.noiseThreshold)
                    chunkRenderer.chunk->setBlock(pos, Block{DIRT, noiseV});
                else
                    chunkRenderer.chunk->setBlock(pos, Block{AIR, noiseV});
            }
        }
    }

    gameRenderer.setupRender();
    chunkRenderer.setupRender();
    debugRenderer.setupRender();
    camera.setup();

    /// Main loop

    double time;
    double deltaTime;
    while (!glfwWindowShouldClose(window)) {
        time = glfwGetTime();

        camera.processInput(deltaTime);
        gameRenderer.render(deltaTime);
        chunkRenderer.render(deltaTime);
        debugRenderer.render(deltaTime);

        frame++;
        deltaTime = glfwGetTime() - time;

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwTerminate();
}