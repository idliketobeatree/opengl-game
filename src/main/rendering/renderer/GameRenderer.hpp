#pragma once

#include <engine/rendering/rendering.hpp>
#include <engine/rendering/shader/ShaderProgram.hpp>

class GameRenderer : Renderer {
protected:
    ShaderProgram program;

    VAO vao;
    VBO vbo;

    UniformLoc vpLoc;
public:
    void setupRender();
    void render(double dt);
};