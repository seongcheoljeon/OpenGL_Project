//
// Created by seongcheoljeon on 2026-02-01.
//

#include "texture.h"


TextureUPtr Texture::Create( uint32_t width, uint32_t height, uint32_t format )
{
    auto texture = TextureUPtr(new Texture());
    texture->_CreateTexture();
    texture->_SetTextureFormat(width, height, format);
    texture->SetFilter(GL_LINEAR, GL_LINEAR);
    return texture;
}

TextureUPtr Texture::CreateFromImage( const Image* image )
{
    auto texture = TextureUPtr(new Texture());
    texture->_CreateTexture();
    texture->_SetTextureFromImage(image);
    return texture;
}

TextureUPtr Texture::CreateFromImage( uint32_t width, uint32_t height, uint32_t format )
{
    auto texture = TextureUPtr(new Texture());
    texture->_CreateTexture();
    texture->_SetTextureFormat(width, height, format);
    texture->SetFilter(GL_LINEAR, GL_LINEAR);
    return texture;
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
    SetFilter(GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
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

    _width  = image->GetWidth();
    _height = image->GetHeight();
    _format = format;

    glTexImage2D(GL_TEXTURE_2D, 0, _format, _width, _height
                 , 0, format, GL_UNSIGNED_BYTE, image->GetData());

    glGenerateMipmap(GL_TEXTURE_2D);
}

void Texture::_SetTextureFormat( uint32_t width, uint32_t height, uint32_t format )
{
    _width  = width;
    _height = height;
    _format = format;

    glTexImage2D(
        GL_TEXTURE_2D, 0, _format, _width, _height
        , 0, _format, GL_UNSIGNED_BYTE, nullptr);
}