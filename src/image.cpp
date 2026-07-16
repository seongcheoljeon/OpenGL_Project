//
// Created by seongcheoljeon on 2026-01-29.
//

#include "image.h"

#define STB_IMAGE_IMPLEMENTATION
#include "stb/stb_image.h"


ImageUPtr Image::Load( const std::string& filepath, bool flip_vertical )
{
    auto image = ImageUPtr(new Image());
    if (!image->_LoadWithStb(filepath, flip_vertical))
    {
        return nullptr;
    }
    return image;
}

ImageUPtr Image::Create( int width, int height, int channel_count )
{
    auto image = ImageUPtr(new Image());
    if (!image->_Allocate(width, height, channel_count))
    {
        return nullptr;
    }
    return image;
}

ImageUPtr Image::CreateSingleColor( int width, int height, const glm::vec4& color )
{
    glm::vec4 clamped = glm::clamp(color * 255.0f, 0.0f, 255.0f);
    uint8_t rgba[4]   = {
        static_cast<uint8_t>(clamped.r)
        , static_cast<uint8_t>(clamped.g)
        , static_cast<uint8_t>(clamped.b)
        , static_cast<uint8_t>(clamped.a)
    };

    auto image = Create(width, height, 4);
    for (int i = 0; i < width * height; ++i)
    {
        memcpy(image->_data + 4 * i, rgba, 4);
    }

    return image;
}

Image::~Image()
{
    if (_data)
    {
        stbi_image_free(_data);
    }
}

void Image::SetCheckImage( int grid_x, int grid_y )
{
    for (int j = 0; j < _height; j++)
    {
        for (int i = 0; i < _width; i++)
        {
            int pos       = (j * _width + i) * _channel_count;
            bool even     = ((i / grid_x) + (j / grid_y)) % 2 == 0;
            uint8_t value = even ? 255 : 0;
            for (int k = 0; k < _channel_count; k++)
            {
                _data[pos + k] = value;
            }
            if (_channel_count > 3)
            {
                _data[pos + 3] = 255;
            }
        }
    }
}

bool Image::_LoadWithStb( const std::string& filepath, bool flip_vertical )
{
    stbi_set_flip_vertically_on_load(flip_vertical);
    _data = stbi_load(filepath.c_str(), &_width, &_height, &_channel_count, 0);
    if (!_data)
    {
        SPDLOG_ERROR("failed to load image: {}", filepath);
        return false;
    }
    return true;
}

bool Image::_Allocate( int width, int height, int channel_count )
{
    _width         = width;
    _height        = height;
    _channel_count = channel_count;
    _data          = (uint8_t*)malloc(_width * _height * _channel_count);

    return _data != nullptr;
}