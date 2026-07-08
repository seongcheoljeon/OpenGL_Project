#version 330 core

in vec3 normal;
in vec3 texcoord;
out vec4 frag_color;

uniform vec3 light_color;
uniform vec3 object_color;
uniform float ambient_strength;


void main() {
    vec3 ambient = ambient_strength * light_color;
    vec3 result = ambient * object_color;

    frag_color = vec4(result, 1.0);
}