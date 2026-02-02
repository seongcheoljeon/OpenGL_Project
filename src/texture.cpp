//
// Created by seongcheoljeon on 2026-02-01.
//

#include "texture.h"


TextureUPtr Texture::CreateFromImage( const Image* image )
{
    auto texture = TextureUPtr(new Texture());
    texture->_CreateTexture();
    texture->_SetTextureFromImage(image);
    return std::move(texture);
}

Texture::~Texture()
{
    if (_texture)
    {
        glDeleteTextures(1, &_texture);
    }
}

void Texture::Bind() const
{
    glBindTexture(GL_TEXTURE_2D, _texture);
}

void Texture::SetFilter( uint32_t min_filter, uint32_t mag_filter ) const
{
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, min_filter);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, mag_filter);
}

void Texture::SetWrap( uint32_t s_wrap, uint32_t t_wrap ) const
{
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, s_wrap);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, t_wrap);
}

void Texture::_CreateTexture()
{
    glGenTextures(1, &_texture);
    Bind();
    SetFilter(GL_LINEAR, GL_LINEAR);
    SetWrap(GL_CLAMP_TO_EDGE, GL_CLAMP_TO_EDGE);
}

void Texture::_SetTextureFromImage( const Image* image )
{
    GLenum format = GL_RGBA;
    switch (image->GetChannelCount())
    {
    case 1:
        format = GL_RED;
        break;
    case 2:
        format = GL_RG;
        break;
    case 3:
        format = GL_RGB;
        break;
    default:
        break;
    }

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, image->GetWidth(), image->GetHeight()
        , 0, format, GL_UNSIGNED_BYTE, image->GetData());
}