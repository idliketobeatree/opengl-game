#include "ShaderProgram.hpp"

#include <stdarg.h>

#include <logging/logging.hpp>

ShaderProgram::ShaderProgram(ShaderID vertex, ShaderID fragment) {
    createProgram();
    attachShader(vertex);
    attachShader(fragment);
    linkProgram();
}
ShaderProgram::ShaderProgram() {}

void ShaderProgram::createProgram() {
    ID = glCreateProgram();
}
void ShaderProgram::attachShader(ShaderID shader) const {
    glAttachShader(ID, shader);
}
void ShaderProgram::attachShaders(const uint_fast8_t amount, ...) const {
    va_list ap;
    va_start(ap, amount);
    for(uint_fast8_t i=0; i<amount; i++)
        glAttachShader(ID, va_arg(ap, ShaderID));
    va_end(ap);
}
void ShaderProgram::linkProgram() const {
    GLint success;
    glLinkProgram(ID);
    // print linking errors if any
    glGetProgramiv(ID, GL_LINK_STATUS, &success);
    if(!success)
    {
        char infoLog[512];
        glGetProgramInfoLog(ID, 512, NULL, infoLog);
        error("Shader program linking failed. Info log: %s", infoLog);
        return;
    }
}