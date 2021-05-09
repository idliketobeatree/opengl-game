#pragma once

// TODO figure out how to make it so I don't have to make this file have dumb name >:(

#include <engine/rendering/rendering.hpp>
#include <engine/rendering/shader/Shader.hpp>
#include <engine/rendering/shader/ShaderProgram.hpp>
#include <engine/rendering/camera/Camera.hpp>
#include <rendering/camera/GameCamera.hpp>
#include <rendering/renderer/GameRenderer.hpp>

extern GameRenderer gameRenderer;
extern GameCamera camera;