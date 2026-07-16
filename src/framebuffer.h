//
// Created by seongcheoljeon on 2026-07-13.
//

#ifndef OPENGL_PROJECT_FRAMEBUFFER_H
#define OPENGL_PROJECT_FRAMEBUFFER_H

#include "texture.h"

CLASS_PTR(Framebuffer);

class Framebuffer
{
public:
    static FramebufferUPtr Create( const TextureSPtr color_attachment );

    static void BindToDefault();

    ~Framebuffer();

    uint32_t Get() const
    {
        return _framebuffer;
    };

    void Bind() const;

    const TextureSPtr GetColorAttachment() const
    {
        return _color_attachment;
    };

private:
    Framebuffer() = default;

    bool _InitWithColorAttachment( const TextureSPtr color_attachment );

    uint32_t _framebuffer{0};
    uint32_t _depth_stencil_buffer{0};
    TextureSPtr _color_attachment;
};

#endif //OPENGL_PROJECT_FRAMEBUFFER_H