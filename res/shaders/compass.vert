#version 330 core
layout (location = 0) in vec3 position;
layout (location = 1) in vec4 color;

out vec4 vertexColor;

uniform mat4 projection;

void main() {
    gl_Position = projection * vec4(position.x, position.y, position.z, 1.0);
    vertexColor = vec4(color.r, color.g, color.b, 1);
}
