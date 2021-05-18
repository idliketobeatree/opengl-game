#include "DebugRenderer.hpp"

#include <rendering/rendering.hpp>
#include <math/math.hpp>

void DebugRenderer::render(double dt) {
    glUseProgram(program);
    glBindVertexArray(vao);

    Matrix4f view{
            camera.right.x,  camera.up.x, camera.direction.x, 0,
            camera.right.y,  camera.up.y, camera.direction.y, 0,
            camera.right.z,  camera.up.z, camera.direction.z, 0,
            0, 0, 0, 1
    };
    glUniformMatrix4fv(projLoc, 1, GL_FALSE, view);
    glDrawArrays(GL_LINES, 0, 6);
}
void DebugRenderer::setupRender() {
    float vertices[] = {
            -0.2f,  0.0f,  0.0f,  1.0f, 0.0f, 0.0f, 1.0f,
             0.2f,  0.0f,  0.0f,  1.0f, 0.0f, 0.0f, 1.0f,

             0.0f, -0.2f,  0.0f,  0.0f, 1.0f, 0.0f, 1.0f,
             0.0f,  0.2f,  0.0f,  0.0f, 1.0f, 0.0f, 1.0f,

             0.0f,  0.0f, -0.2f,  0.0f, 0.0f, 1.0f, 1.0f,
             0.0f,  0.0f,  0.2f,  0.0f, 0.0f, 1.0f, 1.0f
    };

    Shader vert(GL_VERTEX_SHADER, "res/shaders/compass.vert");
    Shader frag(GL_FRAGMENT_SHADER, "res/shaders/compass.frag");

    program.createProgram();
    program.attachShader(vert);
    program.attachShader(frag);
    program.linkProgram();

    vert.deleteShader();
    frag.deleteShader();

    glGenVertexArrays(1, &vao);
    glGenBuffers(1, &vbo);

    glBindVertexArray(vao);

    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    // coords
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 7 * sizeof(float), (void*) 0);
    glEnableVertexAttribArray(0);
    // colors
    glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 7 * sizeof(float), (void*) (3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    projLoc = glGetUniformLocation(program, "projection");
}
