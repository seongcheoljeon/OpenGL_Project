//
// Created by seongcheoljeon on 2025-12-22.
//

#ifndef OPENGL_PROJECT_CONTEXT_H
#define OPENGL_PROJECT_CONTEXT_H

#include "common.h"
#include "shader.h"
#include "program.h"
#include "buffer.h"
#include "vertex_layout.h"
#include "texture.h"


CLASS_PTR(Context)

class Context
{
public:
    static ContextUPtr Create();

    void Render();

private:
    Context() = default;

    bool _Init();

    ProgramUPtr _program{nullptr};

    VertexLayoutUPtr _vertex_layout{nullptr};
    BufferUPtr _vertex_buffer{nullptr};
    BufferUPtr _index_buffer{nullptr};
    TextureUPtr _texture{nullptr};
};

#endif // OPENGL_PROJECT_CONTEXT_H