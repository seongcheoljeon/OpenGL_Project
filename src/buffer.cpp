//
// Created by seongcheoljeon on 2026-01-24.
//

#include "buffer.h"


BufferUPtr Buffer::CreateWithData( uint32_t buffer_type, uint32_t usage, const void* data, size_t stride, size_t count )
{
    auto buffer = BufferUPtr(new Buffer());
    if (!buffer->_Init(buffer_type, usage, data, stride, count))
    {
        return nullptr;
    }
    return buffer;
}

Buffer::~Buffer()
{
    if (_buffer)
    {
        glDeleteBuffers(1, &_buffer);
    }
}

void Buffer::Bind() const
{
    glBindBuffer(_buffer_type, _buffer);
}

bool Buffer::_Init( uint32_t buffer_type, uint32_t usage, const void* data, size_t stride, size_t count )
{
    _buffer_type = buffer_type;
    _usage       = usage;
    _stride      = stride;
    _count       = count;
    glGenBuffers(1, &_buffer);
    this->Bind();
    glBufferData(_buffer_type, _stride * _count, data, usage);

    return true;
}