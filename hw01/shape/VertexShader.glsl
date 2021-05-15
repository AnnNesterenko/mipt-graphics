#version 330 core

layout(location = 0) in vec3 vs_pos;
layout(location = 1) in vec4 vs_col;

out vec4 fs_col;

uniform mat4 transform;

void main() {
    gl_Position = transform * vec4(vs_pos, 1);
    fs_col = vs_col;
}