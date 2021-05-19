#version 330 core
layout (location = 0) in vec3 position;
layout (location = 1) in vec4 color;

out vec4 vertexColor;
out float depth;

uniform mat4 vp;

void main() {
    gl_Position = vp * vec4(position.x, position.y, position.z, 1.0);
    depth = gl_Position.z;
    vertexColor = color;
}
