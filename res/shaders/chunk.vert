#version 330 core
layout (location = 0) in vec3 a_pos;
layout (location = 1) in vec3 a_normal;
layout (location = 2) in vec3 a_color;

out vec3 pos;
out vec3 normal;
out vec3 color;

uniform mat4 u_mvp;
uniform mat4 u_view;

void main() {
    pos = a_pos;
    normal = a_normal;
    color = a_color;

    gl_Position = u_mvp * vec4(a_pos, 1.0);
}