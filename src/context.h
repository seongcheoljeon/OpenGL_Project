//
// Created by seongcheoljeon on 2025-12-22.
//

#ifndef OPENGL_PROJECT_CONTEXT_H
#define OPENGL_PROJECT_CONTEXT_H

#include "common.h"
#include "shader.h"
#include "program.h"
#include "buffer.h"

CLASS_PTR(Context)

class Context
{
public:
    static ContextUPtr Create();

    void Render();

private:
    Context() = default;

    bool _Init();

    ProgramUPtr _program = nullptr;

    uint32_t _vertex_array_object = 0;

    BufferUPtr _vertex_buffer = 0;
    BufferUPtr _index_buffer  = 0;
    /*
    uint32_t _vertex_buffer = 0;
    uint32_t _index_buffer = 0;
*/
};

#endif // OPENGL_PROJECT_CONTEXT_H