#pragma once

#include <engine/rendering/rendering.hpp>
#include <engine/rendering/shader/ShaderProgram.hpp>

class GameRenderer : Renderer {
protected:
    ShaderProgram program;
    UniformLoc vpLoc;

    VAO vao;
    VBO vbo;
public:
    void setupRender();
    void render(double dt);
};