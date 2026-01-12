// 330 core 부터 동작할 수 있도록
#version 330 core

layout (location = 0) in vec3 a_pos;

void main() {
    gl_Position = vec4(0.0, 0.0, 0.0, 1.0);
}