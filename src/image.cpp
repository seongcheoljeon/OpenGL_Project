//
// Created by seongcheoljeon on 2026-01-29.
//

#include "image.h"

#define STB_IMAGE_IMPLEMENTATION
#include "stb/stb_image.h"


ImageUPtr Image::Load( const std::string& filepath )
{
    auto image = ImageUPtr(new Image());
    if (!image->_LoadWithStb(filepath))
    {
        return nullptr;
    }
    return std::move(image);
}

ImageUPtr Image::Create( int width, int height, int channel_count )
{
    auto image = ImageUPtr(new Image());
    if (!image->_Allocate(width, height, channel_count))
    {
        return nullptr;
    }
    return std::move(image);
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
    for (int j=0; j<_height; j++)
    {
        for (int i=0; i<_width; i++)
        {
            int pos = (j * _width + i) * _channel_count;
            bool even = ((i / grid_x) + (j / grid_y)) % 2 == 0;
            uint8_t value = even ? 255 : 0;
            for (int k=0; k<_channel_count; k++)
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

bool Image::_LoadWithStb( const std::string& filepath )
{
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
    _width = width;
    _height = height;
    _channel_count = channel_count;
    _data = (uint8_t*)malloc(_width * _height * _channel_count);

    return _data ? true : false;
}