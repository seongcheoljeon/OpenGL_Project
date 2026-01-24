//
// Created by seongcheoljeon on 2025-12-22.
//

#ifndef OPENGL_PROJECT_PROGRAM_H
#define OPENGL_PROJECT_PROGRAM_H

#include "common.h"
#include "shader.h"

CLASS_PTR(Program)

class Program
{
public:
    static ProgramUPtr Create(const std::vector<ShaderSPtr>& shaders);

    ~Program();

    [[nodiscard]] uint32_t Get() const
    {
        return _program;
    }
    void Use() const;

private:
    Program() = default;
    bool _Link(const std::vector<ShaderSPtr>& shaders);
    uint32_t _program {0};
};

#endif // OPENGL_PROJECT_PROGRAM_H
