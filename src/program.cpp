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
    if (_program)
    {
        glDeleteProgram(_program);
        _program = 0;
    }
}

void Program::Use() const
{
    glUseProgram(_program);
}

bool Program::_Link(const std::vector<ShaderSPtr> &shaders)
{
    // OpenGL program object 생성
    _program = glCreateProgram();
    for (const ShaderSPtr& shader: shaders)
    {
        // program에 shader 붙이기
        glAttachShader(_program, shader->Get());
    }
    // program 링크
    glLinkProgram(_program);

    int success = 0;
    // program에 대한 정수형 정보를 얻어옴
    glGetProgramiv(_program, GL_LINK_STATUS, &success);
    if (!success)
    {
        char info_log[1024];
        // program에 대한 로그를 얻어옴. 링크 에러 얻어내는 용도로 사용
        glGetProgramInfoLog(_program, 1024, nullptr, info_log);
        SPDLOG_ERROR("failed to link program: {}", info_log);
        return false;
    }
    return true;
}