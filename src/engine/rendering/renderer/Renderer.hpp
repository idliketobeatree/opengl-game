#pragma once

#include <engine/rendering/rendering.hpp>

struct Renderer {
    void setupRender();
    void render(double dt);
};