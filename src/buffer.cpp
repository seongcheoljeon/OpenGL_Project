//
// Created by seongcheoljeon on 2026-01-24.
//

#include "buffer.h"


BufferUPtr Buffer::CreateWithData( uint32_t buffer_type, uint32_t usage, const void* data, size_t data_size )
{
    auto buffer = BufferUPtr(new Buffer());
    if (!buffer->Init(buffer_type, usage, data, data_size))
    {
        return nullptr;
    }
    return std::move(buffer);
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

bool Buffer::Init( uint32_t buffer_type, uint32_t usage, const void* data, size_t data_size )
{
    _buffer_type = buffer_type;
    _usage = usage;
    glGenBuffers(1, &_buffer);
    this->Bind();
    glBufferData(_buffer_type, data_size, data, usage);

    return true;
}
