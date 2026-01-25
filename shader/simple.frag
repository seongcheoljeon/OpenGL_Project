#version 330 core

uniform vec4 color;
out vec4 frag_color; // 최종 출력 색상


void main() {
    frag_color = color;
}