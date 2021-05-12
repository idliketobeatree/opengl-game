#include "GameRenderer.hpp"

#include <engine/logging/logging.hpp>

#include <engine/math/math.hpp>
#include <rendering/rendering.hpp>

void GameRenderer::render(double dt) {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    Matrix4<GLfloat> vp = camera.view *camera.projection;

    glUniformMatrix4fv(vpLoc, 1, GL_FALSE, vp);

    glDrawArrays(GL_TRIANGLES, 0, 21);
}

void GameRenderer::setupRender() {
    float vertices[] = {
            0.0f,  0.5f, 0.0f,  0.0f, 0.0f, 1.0f, 1.0f,
            0.5f, -0.5f, 0.0f,  0.0f, 1.0f, 0.0f, 1.0f,
            -0.5f, -0.5f, 0.0f,  1.0f, 0.0f, 0.0f, 1.0f
    };

    glEnable(GL_DEPTH_TEST);
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    glEnable(GL_FRAMEBUFFER_SRGB);

//    glEnable(GL_CULL_FACE);
//    glCullFace(GL_FRONT);
//    glFrontFace(GL_CCW);


    Shader vert(GL_VERTEX_SHADER, "res/shaders/test.vert");
    Shader frag(GL_FRAGMENT_SHADER, "res/shaders/test.frag");

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

    glUseProgram(program);

    vpLoc = glGetUniformLocation(program, "vp");
}