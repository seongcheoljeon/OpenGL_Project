// 330 core 부터 동작할 수 있도록
#version 330 core

// location = 0 이 곧 VAO Attribute 0번을 의미. 0번째는 정점 위치 데이터
layout (location = 0) in vec3 a_pos;


void main() {
    gl_Position = vec4(a_pos, 1.0);
}
