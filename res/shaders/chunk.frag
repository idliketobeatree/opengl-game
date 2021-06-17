#version 330 core
struct PointLight {
    vec3 position;

    vec3 ambientColor, diffuseColor, specularColor;
    float ambientStrength, diffuseStrength, specularStrength;

    float constant, linear, quadratic;
};
struct DirectionalLight {
    vec3 direction;

    vec3 ambientColor, diffuseColor, specularColor;
    float ambientStrength, diffuseStrength, specularStrength;
};
struct Spotlight {
    vec3 position, direction;
    float cutoff, outerCutoff;

    vec3 ambientColor, diffuseColor, specularColor;
    float ambientStrength, diffuseStrength, specularStrength;

    float constant, linear, quadratic;
};

out vec4 fragColor;

in vec3 pos;
in vec3 normal;
in vec3 color;

uniform DirectionalLight sun;
uniform Spotlight flashlight;

uniform vec3 u_viewPos;

vec3 point(PointLight light);
vec3 directional(DirectionalLight light);
vec3 spot(Spotlight light);

void main() {
//    directional();
//    point();
//    spot();
    fragColor = vec4((directional(sun) + spot(flashlight)) * color, 1);
}


vec3 point(PointLight light) {
    float distance  = length(flashlight.position - pos);
    vec3 lightDir   = normalize(flashlight.position - pos);
    vec3 viewDir    = normalize(u_viewPos - pos);
    vec3 reflectDir = reflect(-lightDir, normal);

    float spec = pow(max(dot(viewDir, reflectDir), 0.0), 128);
    float diff = max(dot(normal, lightDir), 0.0);

    float attenuation = 1.0 / (flashlight.constant + flashlight.linear * distance +
    flashlight.quadratic * (distance * distance));
    vec3 specular = flashlight.specularStrength * spec * flashlight.specularColor  * attenuation;
    vec3 diffuse  = diff * flashlight.diffuseStrength * flashlight.diffuseColor * attenuation;
    vec3 ambient  = flashlight.ambientStrength * flashlight.ambientColor;

    return ambient + diffuse + specular;
}
vec3 directional(DirectionalLight light) {
    vec3 lightDir   = normalize(-light.direction);
    vec3 viewDir    = normalize(u_viewPos - pos);
    vec3 reflectDir = reflect(-lightDir, normal);

    float spec = pow(max(dot(viewDir, reflectDir), 0.0), 128);
    float diff = max(dot(normal, lightDir), 0.0);

    vec3 specular = light.specularStrength * spec * light.specularColor;
    vec3 diffuse  = diff * light.diffuseStrength * light.diffuseColor;
    vec3 ambient  = light.ambientStrength * light.ambientColor;

    return ambient + diffuse + specular;
}
vec3 spot(Spotlight light) {
    vec3 lightDir   = normalize(light.position - pos);
    float theta     = dot(lightDir, normalize(light.direction));
    float epsilon   = light.cutoff - light.outerCutoff;
    float intensity = clamp((theta - light.outerCutoff) / epsilon, 0.0, 1.0);

    if(theta > light.outerCutoff) {
        float distance  = length(light.position - pos);
        vec3 viewDir    = normalize(u_viewPos - pos);
        vec3 reflectDir = reflect(-lightDir, normal);

        float spec = pow(max(dot(viewDir, reflectDir), 0.0), 128);
        float diff = max(dot(normal, lightDir), 0.0);

        float attenuation = 1.0 / (light.constant + light.linear * distance +
        light.quadratic * (distance * distance));
        vec3 specular = light.specularStrength * spec * light.specularColor  * attenuation * intensity;
        vec3 diffuse  = diff * light.diffuseStrength * light.diffuseColor * attenuation * intensity;
        vec3 ambient  = light.ambientStrength * light.ambientColor;

        return ambient + diffuse + specular;
    } else {
        vec3 ambient  = light.ambientStrength * light.ambientColor;
        return ambient;
    }
}