#version 460 core

layout (location = 0) in vec3 a_pos;
layout (location = 1) in vec3 a_normal;
layout (location = 2) in vec2 a_texcoord;

uniform mat4 transform;
uniform mat4 model_transform;

out vec3 normal;
out vec2 texcoord;
out vec3 position;


void main() {
    gl_Position = transform * vec4(a_pos, 1.0);
    normal = (transpose(inverse(model_transform)) * vec4(a_normal, 0.0)).xyz;
    texcoord = a_texcoord;
    position = (model_transform * vec4(a_pos, 1.0)).xyz;
}