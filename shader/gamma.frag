#version 460 core

in vec4 vertex_color;
in vec2 texcoord;
out vec4 frag_color;

uniform sampler2D tex;
uniform float gamma;


void main()
{
    vec4 pixel = texture(tex, texcoord);
    frag_color = vec4(pow(pixel.rgb, vec3(gamma)), 1.0);
}