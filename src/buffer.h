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
        uint32_t buffer_type, uint32_t usage, const void* data, size_t stride, size_t count );

    ~Buffer();

    [[nodiscard]] uint32_t Get() const
    {
        return _buffer;
    };

    [[nodiscard]] size_t GetStride() const
    {
        return _stride;
    };

    [[nodiscard]] size_t GetCount() const
    {
        return _count;
    }

    void Bind() const;

private:
    Buffer() = default;

    bool _Init( uint32_t buffer_type, uint32_t usage, const void* data, size_t stride, size_t count );

    uint32_t _buffer{0};
    uint32_t _buffer_type{0};
    uint32_t _usage{0};

    size_t _stride{0};
    size_t _count{0};
};

#endif //OPENGL_PROJECT_BUFFER_H