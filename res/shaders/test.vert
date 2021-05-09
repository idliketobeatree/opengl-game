#version 330 core
layout (location = 0) in vec3 position;
layout (location = 1) in vec4 color;

out vec4 vertexColor;

uniform mat4 vp;

#define distance 3

void main() {
    gl_Position = vec4(position.x, position.y, position.z, 1.0)*vp;
//    gl_Position = vec4(position.x, position.y, position.z, 1.0);
//    vertexColor = vec4(
//    ((distance-gl_Position.z)/distance)*color.r,
//    ((distance-gl_Position.z)/distance)*color.g,
//    ((distance-gl_Position.z)/distance)*color.b, 1);
    vertexColor = vec4(color.r, color.g, color.b, 1);
}
