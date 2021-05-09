#pragma once

#include <engine/rendering/typedefs.hpp>

class Shader {
public:
    ShaderID ID;
    GLenum type;

    char *code;
    GLint codeLength;

    Shader(GLenum type, const char *path);

    Shader();
    ~Shader();

    void readFromFile(const char *path);

    void createShader();
    void compile();

    void deleteShader() const;

    inline operator ShaderID() {
        return ID;
    }
};
