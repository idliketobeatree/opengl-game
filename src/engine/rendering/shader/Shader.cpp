#include "Shader.hpp"

#include <logging/logging.hpp>

Shader::Shader() {}
Shader::Shader(GLenum type, const char* path): type(type) {
    createShader();
    readFromFile(path);
    compile();
}
Shader::~Shader() {
    delete[] code;
    deleteShader();
}
void Shader::readFromFile(const char *path) {
    FILE* file = fopen(path, "r");

    fseek(file, 0, SEEK_END);
    codeLength = ftell(file);

    code = new char[codeLength+1];

    rewind(file);
    fread(code, sizeof(char), codeLength, file);
    fclose(file);
    code[codeLength] = '\0';
}
void Shader::compile() {
    GLint success;
    glShaderSource(ID, 1, &code, NULL);
    glCompileShader(ID);

    // print compile errors if any
    glGetShaderiv(ID, GL_COMPILE_STATUS, &success);
    if (!success) {
        char infoLog[512];
        glGetShaderInfoLog(ID, 512, NULL, infoLog);
        error("Shader compilation failed. Info log:\n\n%s\n", infoLog);
        return;
    }
}
void Shader::createShader() {
    ID = glCreateShader(type);
}
void Shader::deleteShader() const {
    glDeleteShader(ID);
}