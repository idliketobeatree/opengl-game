#version 330 core
out vec4 fragColor;

in vec3 pos;
in vec3 normal;
in vec3 color;

uniform vec3 u_lightPos;
uniform vec3 u_lightColor;
uniform float u_lightStrength;

uniform vec3 u_ambientLightColor;
uniform float u_ambientLightStrength;

uniform vec3 u_viewPos;

void main()
{
    /// should already be normalized, but just in case for testing purposes.
    vec3 norm = normalize(normal);
    vec3 lightDir = normalize(u_lightPos - pos);

    float specularStrength = 1.25;
    vec3 viewDir = normalize(u_viewPos - pos);
    vec3 reflectDir = reflect(-lightDir, norm);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), 128);
    vec3 specular = specularStrength * spec * u_lightColor;

    float diff = max(dot(norm, lightDir), 0.0);

    vec3 diffuse = diff * u_lightStrength * u_lightColor;
    vec3 ambient = u_ambientLightStrength * u_ambientLightColor;

//    fragColor = vec4((ambient + specular) * color, 1);
    fragColor = vec4((ambient + diffuse + specular) * color, 1);
//    fragColor = vec4((ambient + diffuse) * color, 1);
}