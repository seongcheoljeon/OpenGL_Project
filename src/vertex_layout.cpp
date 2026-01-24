//
// Created by seongcheoljeon on 2026-01-24.
//

#include "vertex_layout.h"


VertexLayoutUPtr VertexLayout::Create()
{
    auto vertex_layout = VertexLayoutUPtr(new VertexLayout());
    vertex_layout->_Init();
    return std::move(vertex_layout);
}

VertexLayout::~VertexLayout()
{
    if (_vertex_array_object)
    {
        glDeleteVertexArrays(1, &_vertex_array_object);
    }
}

void VertexLayout::Bind() const
{
    glBindVertexArray(_vertex_array_object);
}

void VertexLayout::SetAttrib( uint32_t attrib_index, int count, uint32_t type, bool normalized, size_t stride
    , uint64_t offset ) const
{
    // 0번 vertex 어트리뷰트 사용
    glEnableVertexAttribArray(attrib_index);
    // 버퍼에서 어트리뷰트 데이터 읽는 방법 설정
    // ex) 위치 데이터는 3개의 float로 구성되어 있음. 정규화된 값이 아니고, 각 정점 데이터는 sizeof(float) * 3 간격으로 떨어져 있음
    glVertexAttribPointer(attrib_index, count, type, normalized, stride, reinterpret_cast<const void*>(offset));
}

void VertexLayout::DisableAttrib( int attrib_index ) const
{
    glDisableVertexAttribArray(attrib_index);
}

void VertexLayout::_Init()
{
    // vertex array object 생성 및 바인딩
    // VAO는 여러 버퍼 바인딩과 어트리뷰트 설정을 하나로 묶어주는 역할
    /*
     * VAO는 VBO에 들어있는 정점 데이터 구조가 어떻게 생겼는지만 알려주는 Reference 같은 존재. vertices[] 자체는 VBO인데,
     * VBO만 가지고는 그 데이터가 어떤 식으로 구성되어 있는지 알 수 없음. 즉, Position/Normal/Color 순으로 되어 있는지 혹은
     * Position/Normal/UV/Color 순으로 되어 있는지 알 수 없음. 그래서 VAO가 필요함.
     */
    glGenVertexArrays(1, &_vertex_array_object);
    Bind();
}