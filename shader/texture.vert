#version 330 core

layout (location = 0) in vec3 a_pos;
layout (location = 1) in vec3 a_color;
layout (location = 2) in vec2 a_texcoord;

uniform mat4 transform;

out vec4 vertex_color;
out vec2 texcoord;

void main()
{
    gl_Position = transform * vec4(a_pos, 1.0);
    vertex_color = vec4(a_color, 1.0);
    texcoord = a_texcoord;
}
