//
// Created by seongcheoljeon on 2026-02-01.
//

#ifndef OPENGL_PROJECT_TEXTURE_H
#define OPENGL_PROJECT_TEXTURE_H

#include "image.h"

CLASS_PTR(Texture)

class Texture
{
public:
    static TextureUPtr Create(uint32_t width, uint32_t height, uint32_t format);
    static TextureUPtr CreateFromImage( const Image* image );

    static TextureUPtr CreateFromImage( uint32_t width, uint32_t height, uint32_t format );

    ~Texture();

    uint32_t Get() const
    {
        return _texture;
    }

    void Bind() const;

    void SetFilter( uint32_t min_filter, uint32_t mag_filter ) const;

    void SetWrap( uint32_t s_wrap, uint32_t t_wrap ) const;

    uint32_t GetWidth() const
    {
        return _width;
    }

    uint32_t GetHeight() const
    {
        return _height;
    }

    uint32_t GetFormat() const
    {
        return _format;
    }

private:
    Texture() = default;

    void _CreateTexture();

    void _SetTextureFromImage( const Image* image );

    void _SetTextureFormat(uint32_t width, uint32_t height, uint32_t format);

private:
    uint32_t _texture = 0;

    uint32_t _width{0};
    uint32_t _height{0};
    uint32_t _format{GL_RGBA};
};


#endif //OPENGL_PROJECT_TEXTURE_H