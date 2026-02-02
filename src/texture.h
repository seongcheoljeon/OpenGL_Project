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
    static TextureUPtr CreateFromImage(const Image* image);
    ~Texture();

    const uint32_t Get() const { return _texture; }
    void Bind() const;
    void SetFilter(uint32_t min_filter, uint32_t mag_filter) const;
    void SetWrap(uint32_t s_wrap, uint32_t t_wrap) const;

private:
    Texture() = default;
    void _CreateTexture();
    void _SetTextureFromImage(const Image* image);

private:
    uint32_t _texture = 0;
};


#endif //OPENGL_PROJECT_TEXTURE_H