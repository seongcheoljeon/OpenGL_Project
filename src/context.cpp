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
    glDrawArrays(GL_POINTS, 0, 1);
}

bool Context::_Init()
{
    ShaderSPtr vertex_shader = Shader::CreateFromFile("./shader/simple.vert", GL_VERTEX_SHADER);
    ShaderSPtr fragment_shader = Shader::CreateFromFile("./shader/simple.frag", GL_FRAGMENT_SHADER);
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

    // temp
    uint32_t vao = 0;
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);
    //

    return true;
}