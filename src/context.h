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
    ProgramUPtr _m_program = nullptr;

    uint32_t _m_vertex_array_object = 0;
    uint32_t _m_vertex_buffer = 0;
    uint32_t _m_index_buffer = 0;
};

#endif // OPENGL_PROJECT_CONTEXT_H
