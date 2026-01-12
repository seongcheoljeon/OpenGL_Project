//
// Created by seongcheoljeon on 2025-12-22.
//

#include "program.h"


ProgramUPtr Program::Create(const std::vector<ShaderSPtr> &shaders)
{
    ProgramUPtr program = ProgramUPtr(new Program());
    if (!program->_Link(shaders))
    {
        return nullptr;
    }
    return std::move(program);
}

Program::~Program()
{
    if (_m_program)
    {
        glDeleteProgram(_m_program);
        _m_program = 0;
    }
}

bool Program::_Link(const std::vector<ShaderSPtr> &shaders)
{
    // OpenGL program object 생성
    _m_program = glCreateProgram();
    for (const ShaderSPtr& shader: shaders)
    {
        // program에 shader 붙이기
        glAttachShader(_m_program, shader->Get());
    }
    // program 링크
    glLinkProgram(_m_program);

    int success = 0;
    // program에 대한 정수형 정보를 얻어옴
    glGetProgramiv(_m_program, GL_LINK_STATUS, &success);
    if (!success)
    {
        char info_log[1024];
        // program에 대한 로그를 얻어옴. 링크 에러 얻어내는 용도로 사용
        glGetProgramInfoLog(_m_program, 1024, nullptr, info_log);
        SPDLOG_ERROR("failed to link program: {}", info_log);
        return false;
    }
    return true;
}