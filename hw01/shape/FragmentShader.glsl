#version 330 core

in vec4 fs_col;

out vec4 col;

void main() {
    col = fs_col;
}