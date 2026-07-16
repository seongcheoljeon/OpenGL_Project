#version 460 core

layout (location = 0) in vec3 a_pos;
out vec3 texcoord;

uniform mat4 transform;


void main() {
    texcoord = a_pos;
    gl_Position = transform * vec4(a_pos, 1.0);
}