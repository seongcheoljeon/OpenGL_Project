//
// Created by seongcheoljeon on 2026-07-11.
//

#ifndef OPENGL_PROJECT_MESH_H
#define OPENGL_PROJECT_MESH_H

#include "common.h"
#include "buffer.h"
#include "vertex_layout.h"


struct Vertex
{
    glm::vec3 position;
    glm::vec3 normal;
    glm::vec2 texcoord;
};

CLASS_PTR(Mesh)

class Mesh
{
public:
    static MeshUPtr Create( const std::vector<Vertex>& vertices, const std::vector<uint32_t>& indices
                            , uint32_t primitive_type );

    static MeshUPtr CreateBox();

    [[nodiscard]] const VertexLayout* GetVertexLayout() const
    {
        return _vertex_layout.get();
    }

    [[nodiscard]] BufferSPtr GetVertexBuffer() const
    {
        return _vertex_buffer;
    }

    [[nodiscard]] BufferSPtr GetIndexBuffer() const
    {
        return _index_buffer;
    }

    void Draw() const;

private:
    Mesh() = default;

    void _Init( const std::vector<Vertex>& vertices, const std::vector<uint32_t>& indices, uint32_t primitive_type );

private:
    uint32_t _primitive_type{GL_TRIANGLES};
    VertexLayoutUPtr _vertex_layout;
    BufferSPtr _vertex_buffer;
    BufferSPtr _index_buffer;
};


#endif //OPENGL_PROJECT_MESH_H