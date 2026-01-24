//
// Created by seongcheoljeon on 2026-01-24.
//

#ifndef OPENGL_PROJECT_VERTEX_LAYOUT_H
#define OPENGL_PROJECT_VERTEX_LAYOUT_H

#include "common.h"

CLASS_PTR(VertexLayout)


class VertexLayout
{
public:
    static VertexLayoutUPtr Create();
    ~VertexLayout();

    uint32_t Get() const { return _vertex_array_object; }
    void Bind() const;
    void SetAttrib(
        uint32_t attrib_index, int count, uint32_t type, bool normalized, size_t stride, uint64_t offset) const;
    void DisableAttrib(int attrib_index) const;

private:
    VertexLayout() = default;
    void _Init();
    uint32_t _vertex_array_object{0};
};

#endif //OPENGL_PROJECT_VERTEX_LAYOUT_H