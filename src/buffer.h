//
// Created by seongcheoljeon on 2026-01-24.
//

#ifndef OPENGL_PROJECT_BUFFER_H
#define OPENGL_PROJECT_BUFFER_H

#include "common.h"


CLASS_PTR(Buffer)

class Buffer
{
public:
    static BufferUPtr CreateWithData(
        uint32_t buffer_type, uint32_t usage, const void* data, size_t data_size );

    ~Buffer();

    uint32_t Get() const
    {
        return _buffer;
    };

    void Bind() const;

private:
    Buffer() = default;

    bool Init( uint32_t buffer_type, uint32_t usage, const void* data, size_t data_size );

    uint32_t _buffer{0};
    uint32_t _buffer_type{0};
    uint32_t _usage{0};
};

#endif //OPENGL_PROJECT_BUFFER_H