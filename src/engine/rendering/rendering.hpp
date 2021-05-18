#pragma once

#include <stdint.h>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <engine/rendering/window.hpp>
#include <engine/rendering/renderer/Renderer.hpp>
#include <engine/rendering/shader/ShaderProgram.hpp>
#include <engine/rendering/shader/Shader.hpp>
#include <engine/rendering/camera/Camera.hpp>

#include "typedefs.hpp"

extern Window window;
extern uint64_t frame;