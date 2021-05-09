#pragma once

#include <engine/rendering/typedefs.hpp>
#include "Shader.hpp"

class ShaderProgram {
public:
    ShaderProgramID ID;

    ShaderProgram(ShaderID vertex, ShaderID fragment);
    ShaderProgram();

    void createProgram();
    void attachShader(ShaderID shader) const;
    void attachShaders(const uint_fast8_t amount, ...) const;
    void linkProgram() const;

    inline operator ShaderProgramID() {
        return ID;
    }
};