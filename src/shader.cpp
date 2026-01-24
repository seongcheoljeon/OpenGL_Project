//
// Created by seongcheoljeon on 2025-12-19.
//

#include "shader.h"

ShaderUPtr Shader::CreateFromFile(const std::string &filename, GLenum shader_type)
{
    ShaderUPtr shader = ShaderUPtr(new Shader());
    if (!shader->_LoadFile(filename, shader_type))
    {
        return nullptr;
    }
    return std::move(shader);
}

Shader::~Shader()
{
    if (_shader)
    {
        glDeleteShader(_shader);
    }
}

bool Shader::_LoadFile(const std::string &filename, GLenum shader_type)
{
    const std::optional<std::string> result = LoadTextFile(filename);
    if (!result.has_value())
    {
        return false;
    }

    const std::string& code = result.value();
    const char* code_ptr = code.c_str();
    const int32_t code_length = static_cast<int32_t>(code.length());

    // create and compile shader
    _shader = glCreateShader(shader_type);
    glShaderSource(_shader, 1, static_cast<const GLchar* const*>(&code_ptr), &code_length);
    glCompileShader(_shader);

    // check compile error
    int32_t success = 0;
    glGetShaderiv(_shader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        char info_log[1024];
        glGetShaderInfoLog(_shader, 1024, nullptr, info_log);
        SPDLOG_ERROR("failed to compile shader: \"{}\"", filename);
        SPDLOG_ERROR("reason: {}", info_log);
        return false;
    }

    return true;
}