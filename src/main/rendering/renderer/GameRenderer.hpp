#pragma once

#include <engine/rendering/rendering.hpp>
#include <engine/rendering/shader/ShaderProgram.hpp>
#include "ChunkRenderer.hpp"

class GameRenderer : Renderer {
protected:
    ChunkRenderer chunkRenderer;
public:
    GameRenderer();

    void setupRender();
    void render(double dt);
};