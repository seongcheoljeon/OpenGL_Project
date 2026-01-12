//
// Created by seongcheoljeon on 2025-12-22.
//

#ifndef OPENGL_PROJECT_CONTEXT_H
#define OPENGL_PROJECT_CONTEXT_H

#include "common.h"
#include "shader.h"
#include "program.h"

CLASS_PTR(Context)

class Context
{
public:
    static ContextUPtr Create();
    void Render();

private:
    Context() = default;
    bool _Init();
    ProgramUPtr _m_program;
};

#endif // OPENGL_PROJECT_CONTEXT_H
