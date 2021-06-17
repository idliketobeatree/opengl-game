#include "Light.hpp"

#include <cstring>

Light::Light(const char *name): name(name) {}
Light::Light(const char *name, ShaderProgram program):name(name) {}
PointLight::PointLight(const char *name): Light(name) {}
PointLight::PointLight(const char *name, ShaderProgram program): Light(name) {
    this->getLocs(program);
}

DirectionalLight::DirectionalLight(const char *name): Light(name) {}
DirectionalLight::DirectionalLight(const char *name, ShaderProgram program): Light(name) {
    this->getLocs(program);
}

Spotlight::Spotlight(const char *name): Light(name) {}
Spotlight::Spotlight(const char *name, ShaderProgram program): Light(name) {
    this->getLocs(program);
}


UniformLoc Light::getLoc(ShaderProgram program, const char *id) {
    char buffer[strlen(name) + strlen(id)];
    strcpy(buffer, name);
    strcat(buffer, id);
    return glGetUniformLocation(program, buffer);
}
void PointLight::getLocs(ShaderProgram program) {
    this->positionLoc          = getLoc(program, ".position");

    this->constantLoc          = getLoc(program, ".constant");
    this->linearLoc            = getLoc(program, ".linear");
    this->quadraticLoc         = getLoc(program, ".quadratic");

    this->ambientColorLoc      = getLoc(program, ".ambientColor");
    this->diffuseColorLoc      = getLoc(program, ".diffuseColor");
    this->specularColorLoc     = getLoc(program, ".specularColor");

    this->ambientStrengthLoc   = getLoc(program, ".ambientStrength");
    this->diffuseStrengthLoc   = getLoc(program, ".diffuseStrength");
    this->specularStrengthLoc  = getLoc(program, ".specularStrength");
}
void DirectionalLight::getLocs(ShaderProgram program) {
    this->directionLoc        = getLoc(program, ".direction");

    this->ambientColorLoc     = getLoc(program, ".ambientColor");
    this->diffuseColorLoc     = getLoc(program, ".diffuseColor");
    this->specularColorLoc    = getLoc(program, ".specularColor");

    this->ambientStrengthLoc  = getLoc(program, ".ambientStrength");
    this->diffuseStrengthLoc  = getLoc(program, ".diffuseStrength");
    this->specularStrengthLoc = getLoc(program, ".specularStrength");
}
void Spotlight::getLocs(ShaderProgram program) {
    this->positionLoc         = getLoc(program, ".position");
    this->directionLoc        = getLoc(program, ".direction");
    this->cutoffLoc           = getLoc(program, ".cutoff");
    this->outerCutoffLoc      = getLoc(program, ".outerCutoff");

    this->constantLoc          = getLoc(program, ".constant");
    this->linearLoc            = getLoc(program, ".linear");
    this->quadraticLoc         = getLoc(program, ".quadratic");

    this->ambientColorLoc     = getLoc(program, ".ambientColor");
    this->diffuseColorLoc     = getLoc(program, ".diffuseColor");
    this->specularColorLoc    = getLoc(program, ".specularColor");

    this->ambientStrengthLoc  = getLoc(program, ".ambientStrength");
    this->diffuseStrengthLoc  = getLoc(program, ".diffuseStrength");
    this->specularStrengthLoc = getLoc(program, ".specularStrength");
}

void PointLight::setAll() {
    glUniform3fv(this->positionLoc, 1, this->position);

    glUniform1f(this->constantLoc, this->constant);
    glUniform1f(this->linearLoc, this->linear);
    glUniform1f(this->quadraticLoc, this->quadratic);

    glUniform3fv(this->ambientColorLoc,  1, this->ambientColor);
    glUniform3fv(this->diffuseColorLoc,  1, this->diffuseColor);
    glUniform3fv(this->specularColorLoc, 1, this->specularColor);

    glUniform1f(this->ambientStrengthLoc,  this->ambientStrength);
    glUniform1f(this->diffuseStrengthLoc,  this->diffuseStrength);
    glUniform1f(this->specularStrengthLoc, this->specularStrength);
}

void DirectionalLight::setAll() {
    glUniform3fv(this->directionLoc, 1, this->direction);

    glUniform3fv(this->ambientColorLoc,  1, this->ambientColor);
    glUniform3fv(this->diffuseColorLoc,  1, this->diffuseColor);
    glUniform3fv(this->specularColorLoc, 1, this->specularColor);

    glUniform1f(this->ambientStrengthLoc,  this->ambientStrength);
    glUniform1f(this->diffuseStrengthLoc,  this->diffuseStrength);
    glUniform1f(this->specularStrengthLoc, this->specularStrength);
}

void Spotlight::setAll() {
    glUniform3fv(this->positionLoc, 1, this->position);
    glUniform3fv(this->directionLoc, 1, this->direction);
    glUniform1f(this->cutoffLoc, this->cutoff);
    glUniform1f(this->outerCutoffLoc, this->outerCutoff);

    glUniform1f(this->constantLoc, this->constant);
    glUniform1f(this->linearLoc, this->linear);
    glUniform1f(this->quadraticLoc, this->quadratic);

    glUniform3fv(this->ambientColorLoc,  1, this->ambientColor);
    glUniform3fv(this->diffuseColorLoc,  1, this->diffuseColor);
    glUniform3fv(this->specularColorLoc, 1, this->specularColor);

    glUniform1f(this->ambientStrengthLoc,  this->ambientStrength);
    glUniform1f(this->diffuseStrengthLoc,  this->diffuseStrength);
    glUniform1f(this->specularStrengthLoc, this->specularStrength);
}