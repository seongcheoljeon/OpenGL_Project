//
// Created by seongcheoljeon on 2026-01-29.
//

#ifndef OPENGL_PROJECT_IMAGE_H
#define OPENGL_PROJECT_IMAGE_H

#include "common.h"

CLASS_PTR(Image)

class Image
{
public:
    static ImageUPtr Load(const std::string& filepath);
    static ImageUPtr Create(int width, int height, int channel_count = 4);
    ~Image();

    const uint8_t* GetData() const { return _data; }
    int GetWidth() const { return _width; }
    int GetHeight() const { return _height; }
    int GetChannelCount() const { return _channel_count; }

    void SetCheckImage(int grid_x, int grid_y);

private:
    Image() = default;
    bool _LoadWithStb(const std::string& filepath);
    bool _Allocate(int width, int height, int channel_count);

private:
    int _width = 0;
    int _height = 0;
    int _channel_count = 0;
    uint8_t* _data = nullptr;
};

#endif //OPENGL_PROJECT_IMAGE_H