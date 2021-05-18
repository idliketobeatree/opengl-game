#pragma once
#include <engine/rendering/renderer/Renderer.hpp>
#include <engine/rendering/shader/ShaderProgram.hpp>

class DebugRenderer: public Renderer {
private:
    ShaderProgram program;
    UniformLoc projLoc;

    VAO vao;
    VBO vbo;
public:
    void setupRender();
    void render(double dt);
};