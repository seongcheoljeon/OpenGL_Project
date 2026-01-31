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

Image::~Image()
{
    if (_data)
    {
        stbi_image_free(_data);
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