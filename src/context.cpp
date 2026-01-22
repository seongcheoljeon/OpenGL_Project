//
// Created by seongcheoljeon on 2025-12-22.
//

#include "context.h"

ContextUPtr Context::Create()
{
    ContextUPtr context = ContextUPtr(new Context());
    if (!context->_Init())
    {
        return nullptr;
    }
    return std::move(context);
}

void Context::Render()
{
    glClear(GL_COLOR_BUFFER_BIT);

    glUseProgram(_m_program->Get());
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);
}

bool Context::_Init()
{
    float vertices[] = {
        // first triangle
        0.5f, 0.5f, 0.0f,
        0.5f, -0.5f, 0.0f,
        -0.5f, -0.5f, 0.0f,
        -0.5f, 0.5f, 0.0f,
    };

    uint32_t indices[] = {
        0, 1, 3, // first triangle
        1, 2, 3, // second triangle
    };

    // 순서 중요!
    // VAO binding -> VBO binding -> vertex attribute 설정

    // vertex array object 생성 및 바인딩
    // VAO는 여러 버퍼 바인딩과 어트리뷰트 설정을 하나로 묶어주는 역할
    glGenVertexArrays(1, &_m_vertex_array_object);
    glBindVertexArray(_m_vertex_array_object);

    // vertex buffer 생성 (position, normal, 등등이 들어가는 버퍼)
    glGenBuffers(1, &_m_vertex_buffer);
    // GL_ARRAY_BUFFER -> Vertex Buffer Object
    // GL_ELEMENT_ARRAY_BUFFER -> Index Buffer Object
    // 버퍼 바인딩
    glBindBuffer(GL_ARRAY_BUFFER, _m_vertex_buffer);
    // GL_STATIC_DRAW -> 버퍼 데이터가 자주 변경되지 않음
    // GL_DYNAMIC_DRAW -> 버퍼 데이터가 자주 변경됨
    // GL_STREAM_DRAW -> 버퍼 데이터가 매 프레임마다 변경됨 (즉, 한번 사용하고 버림)
    // 버퍼에 데이터 업로드
    glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 12, vertices, GL_STATIC_DRAW);

    // 0번 vertex 어트리뷰트 사용
    glEnableVertexAttribArray(0);
    // 버퍼에서 어트리뷰트 데이터 읽는 방법 설정
    // 위치 데이터는 3개의 float로 구성되어 있음. 정규화된 값이 아니고, 각 정점 데이터는 sizeof(float) * 3 간격으로 떨어져 있음
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 3, nullptr);

    // index buffer 생성 및 바인딩
    glGenBuffers(1, &_m_index_buffer);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _m_index_buffer);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(uint32_t) * 6, indices, GL_STATIC_DRAW);

    ShaderSPtr vertex_shader = Shader::CreateFromFile("../shader/simple.vert", GL_VERTEX_SHADER);
    ShaderSPtr fragment_shader = Shader::CreateFromFile("../shader/simple.frag", GL_FRAGMENT_SHADER);
    if (!vertex_shader || !fragment_shader)
    {
        return false;
    }
    SPDLOG_INFO("vertex shader id: {}", vertex_shader->Get());
    SPDLOG_INFO("fragment shader id: {}", fragment_shader->Get());

    _m_program = Program::Create({fragment_shader, vertex_shader});
    if (!_m_program)
    {
        return false;
    }
    SPDLOG_INFO("program id: {}", _m_program->Get());

    glClearColor(0.0f, 0.1f, 0.2f, 0.0f); // color framebuffer 화면을 클리어

    return true;
}