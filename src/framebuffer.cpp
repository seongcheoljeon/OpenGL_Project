//
// Created by seongcheoljeon on 2026-07-13.
//

#include "framebuffer.h"

FramebufferUPtr Framebuffer::Create( const TextureSPtr color_attachment )
{
    auto framebuffer = FramebufferUPtr(new Framebuffer());
    if (!framebuffer->_InitWithColorAttachment(color_attachment))
    {
        return nullptr;
    }
    return framebuffer;
}

void Framebuffer::BindToDefault()
{
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

Framebuffer::~Framebuffer()
{
    if (_depth_stencil_buffer)
    {
        glDeleteRenderbuffers(1, &_depth_stencil_buffer);
    }
    if (_framebuffer)
    {
        glDeleteFramebuffers(1, &_framebuffer);
    }
}

void Framebuffer::Bind() const
{
    glBindFramebuffer(GL_FRAMEBUFFER, _framebuffer);
}

bool Framebuffer::_InitWithColorAttachment( const TextureSPtr color_attachment )
{
    _color_attachment = color_attachment;
    glGenFramebuffers(1, &_framebuffer);
    glBindFramebuffer(GL_FRAMEBUFFER, _framebuffer);

    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, color_attachment->Get(), 0);

    glGenRenderbuffers(1, &_depth_stencil_buffer);
    glBindRenderbuffer(GL_RENDERBUFFER, _depth_stencil_buffer);
    glRenderbufferStorage(
        GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, color_attachment->GetWidth(), color_attachment->GetHeight());
    // default로 돌리는 코드, 어떤 코드든 0을 넣으면 디폴트로 돌리겠다라는 뜻
    glBindRenderbuffer(GL_RENDERBUFFER, 0);

    glFramebufferRenderbuffer(
        GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, _depth_stencil_buffer);

    auto result = glCheckFramebufferStatus(GL_FRAMEBUFFER);
    if (result != GL_FRAMEBUFFER_COMPLETE)
    {
        SPDLOG_ERROR("failed to create framebuffer: {}", result);
        return false;
    }

    BindToDefault();

    return true;
}