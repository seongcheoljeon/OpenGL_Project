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
    SPDLOG_INFO("program id: {}", program->Get());

    return program;
}

ProgramUPtr Program::Create( const std::string& vertex_shader_filename, const std::string& frag_shader_filename )
{
    ShaderSPtr vs = Shader::CreateFromFile(vertex_shader_filename, GL_VERTEX_SHADER);
    ShaderSPtr fs = Shader::CreateFromFile(frag_shader_filename, GL_FRAGMENT_SHADER);
    if (!vs || !fs)
    {
        return nullptr;
    }
    SPDLOG_INFO("vertex shader id: {}", vs->Get());
    SPDLOG_INFO("fragment shader id: {}", fs->Get());
    return Create({vs, fs});
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

void Program::SetUniform( const std::string& name, int value ) const
{
    auto loc = glGetUniformLocation(_program, name.c_str());
    glUniform1i(loc, value);
}

void Program::SetUniform( const std::string& name, float value ) const
{
    auto loc = glGetUniformLocation(_program, name.c_str());
    glUniform1f(loc, value);
}

void Program::SetUniform( const std::string& name, const glm::vec2& value ) const
{
    auto loc = glGetUniformLocation(_program, name.c_str());
    glUniform2fv(loc, 1, glm::value_ptr(value));
}

void Program::SetUniform( const std::string& name, const glm::vec3& value ) const
{
    auto loc = glGetUniformLocation(_program, name.c_str());
    glUniform3fv(loc, 1, glm::value_ptr(value));
}

void Program::SetUniform( const std::string& name, const glm::vec4& value ) const
{
    auto loc = glGetUniformLocation(_program, name.c_str());
    glUniform4fv(loc, 1, glm::value_ptr(value));
}

void Program::SetUniform( const std::string& name, const glm::mat4& value ) const
{
    auto loc = glGetUniformLocation(_program, name.c_str());
    glUniformMatrix4fv(loc, 1, GL_FALSE, glm::value_ptr(value));
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