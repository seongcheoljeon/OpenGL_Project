#version 330 core

uniform vec4 color;
out vec4 frag_color; // 최종 출력 색상

in vec4 vertex_color; // 정점 셰이더에서 전달된 색상


void main()
{
    frag_color = vertex_color;
}