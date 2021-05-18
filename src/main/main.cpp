#include <rendering/rendering.hpp>
#include <logging/logging.hpp>
#include <rendering/renderer/DebugRenderer.hpp>
#include <rendering/renderer/ChunkRenderer.hpp>

#include <math/math.hpp>
#include <world/block/blocks.hpp>

#include <math/Noise.hpp>

Window window(640, 480, "OpenGL Window");
GameRenderer gameRenderer;
DebugRenderer debugRenderer;
ChunkRenderer chunkRenderer(new Chunk({0, 0, 0}));
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
//                float radius = CHUNK_SIZE/2.0f;
//                float xDist = radius - pos.x;
//                float yDist = radius - pos.y;
//                float zDist = radius - pos.z;
//                float noiseV = sqrt (xDist*xDist + yDist*yDist + zDist*zDist);
//                if(noiseV < radius)
//                    chunkRenderer.chunk->setBlock(pos, Block{DIRT, noiseV});
//                else
//                    chunkRenderer.chunk->setBlock(pos, Block{AIR, noiseV});
                float noiseV = noise.noise3f(pos.x/50.0f, pos.y/50.0f, pos.z/50.0f);
                if(noiseV > 0.5)
                    chunkRenderer.chunk->setBlock(pos, Block{DIRT, 1});
                else
                    chunkRenderer.chunk->setBlock(pos, Block{AIR, 1});
//                if(pos.x/4 % 2 == 0)
//                    chunkRenderer.chunk->setBlock(pos, Block{DIRT, 1.0f});
//                else
//                    chunkRenderer.chunk->setBlock(pos, Block{AIR, 1.0f});
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