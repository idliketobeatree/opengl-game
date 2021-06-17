#pragma once

#include <util/math/math.hpp>
#include <engine/rendering/typedefs.hpp>
#include <engine/rendering/shader/ShaderProgram.hpp>

struct Light {
    const char *name;

    Vector3f   ambientColor,       diffuseColor,       specularColor;
    float      ambientStrength,    diffuseStrength,    specularStrength;

    UniformLoc ambientColorLoc,    diffuseColorLoc,    specularColorLoc,
               ambientStrengthLoc, diffuseStrengthLoc, specularStrengthLoc;

    Light(const char *name);
    Light(const char *name, ShaderProgram program);

    void getLocs(ShaderProgram program);
    void setAll();
protected:
    UniformLoc getLoc(ShaderProgram program, const char *id);
};

struct PointLight: Light {
    Vector3f position;
    float constant, linear, quadratic;

    UniformLoc positionLoc,
               constantLoc, linearLoc, quadraticLoc;

    PointLight(const char *name);
    PointLight(const char *name, ShaderProgram program);

    void getLocs(ShaderProgram program);
    void setAll();
};
struct DirectionalLight: Light {
    Vector3f direction;

    UniformLoc directionLoc;

    DirectionalLight(const char *name);
    DirectionalLight(const char *name, ShaderProgram program);

    void getLocs(ShaderProgram program);
    void setAll();
};
struct Spotlight: Light {
    Vector3f position, direction;
    float constant, linear, quadratic;
    float cutoff, outerCutoff;

    UniformLoc positionLoc, directionLoc,
    constantLoc, linearLoc, quadraticLoc,
    cutoffLoc, outerCutoffLoc;

    Spotlight(const char *name);
    Spotlight(const char *name, ShaderProgram program);

    void getLocs(ShaderProgram program);
    void setAll();
};