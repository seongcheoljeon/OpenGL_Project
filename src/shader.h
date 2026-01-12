//
// Created by seongcheoljeon on 2025-12-19.
//

#ifndef OPENGL_PROJECT_SHADER_H
#define OPENGL_PROJECT_SHADER_H

#include "common.h"

CLASS_PTR(Shader)


class Shader
{
public:
    static ShaderUPtr CreateFromFile(const std::string& filename, GLenum shader_type);
    ~Shader();
    uint32_t Get() const { return _m_shader; }

private:
    // 생성자가 private인 이유: CreateFromFile() 함수 외에 다른 방식의 Shader 인스턴스 생성을 막기 위함.
    // 즉, 정적 팩토리 메서드를 통해서만 인스턴스를 생성할 수 있음.
    Shader() = default;
    bool _LoadFile(const std::string& filename, GLenum shader_type);
    uint32_t _m_shader { 0 };
};

#endif // OPENGL_PROJECT_SHADER_H
