#version 460 core

in vec4 vertex_color;
in vec2 texcoord;
out vec4 frag_color;

uniform sampler2D tex;


void main()
{
    vec4 pixel = texture(tex, texcoord);
    if(pixel.a < 0.01)
    {
        discard;
    }

    frag_color = pixel;
}