#version 330 core
in vec4 vertexColor;
in float depth;

out vec4 fragColor;

void main() {
    fragColor = vec4(vertexColor.r * (1-depth/50), vertexColor.g * (1-depth/50), vertexColor.b * (1-depth/50), vertexColor.a);
}