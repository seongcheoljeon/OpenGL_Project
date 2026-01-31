//
// Created by seongcheoljeon on 2025-12-22.
//

#include "context.h"
#include "image.h"

ContextUPtr Context::Create()
{
    ContextUPtr context = ContextUPtr(new Context());
    if (!context->_Init())
    {
        return nullptr;
    }
    return std::move(context);
}

void Context::Render()
{
    glClear(GL_COLOR_BUFFER_BIT);

    _program->Use();
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);
}

bool Context::_Init()
{
    float vertices[] = {
        0.5f, 0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f,
        0.5f, -0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f,
        -0.5f, -0.5f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f,
        -0.5f, 0.5f, 0.0f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f,
    };

    uint32_t indices[] = {
        0, 1, 3, // first triangle
        1, 2, 3, // second triangle
    };

    // 순서 중요!
    // VAO binding -> VBO binding -> vertex attribute 설정

    _vertex_layout = VertexLayout::Create();

    // vertex buffer 생성 (position, normal, 등등이 들어가는 버퍼)
    _vertex_buffer = Buffer::CreateWithData(GL_ARRAY_BUFFER, GL_STATIC_DRAW
        , vertices, sizeof(float) * 32);

    // _vertex_layout->SetAttrib(0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 3, 0);
    _vertex_layout->SetAttrib(0, 3, GL_FLOAT, GL_FALSE
        , sizeof(float) * 8, 0);
    _vertex_layout->SetAttrib(1, 3, GL_FLOAT, GL_FALSE
        , sizeof(float) * 8, sizeof(float) * 3);
    _vertex_layout->SetAttrib(2, 2, GL_FLOAT, GL_FALSE
        , sizeof(float) * 8, sizeof(float) * 6);

    // index buffer 생성 및 바인딩
    _index_buffer = Buffer::CreateWithData(GL_ELEMENT_ARRAY_BUFFER, GL_STATIC_DRAW
        , indices, sizeof(uint32_t) * 6);

    ShaderSPtr vertex_shader   = Shader::CreateFromFile("../shader/simple.vert", GL_VERTEX_SHADER);
    ShaderSPtr fragment_shader = Shader::CreateFromFile("../shader/simple.frag", GL_FRAGMENT_SHADER);
    if (!vertex_shader || !fragment_shader)
    {
        return false;
    }
    SPDLOG_INFO("vertex shader id: {}", vertex_shader->Get());
    SPDLOG_INFO("fragment shader id: {}", fragment_shader->Get());

    _program = Program::Create({fragment_shader, vertex_shader});
    if (!_program)
    {
        return false;
    }
    SPDLOG_INFO("program id: {}", _program->Get());

    glClearColor(0.0f, 0.1f, 0.2f, 0.0f); // color framebuffer 화면을 클리어

    /////////////////
    auto image = Image::Load("../image/container.jpg");
    if (!image)
    {
        return false;
    }
    SPDLOG_INFO("image: {}x:{}, {} channels", image->GetWidth(), image->GetHeight(), image->GetChannelCount());

    // OpenGL texture object 생성
    glGenTextures(1, &_texture);
    // 사용하고자 하는 텍스처 바인딩
    glBindTexture(GL_TEXTURE_2D, _texture);
    // 텍스처 필터/래핑 방식 등 파라미터 설정
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

    // cpu에 있는 이미지 데이터를 gpu 텍스처 메모리로 복사
    // target: 대상이 될 바인딩 텍스처
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, image->GetWidth(), image->GetHeight()
        , 0, GL_RGB, GL_UNSIGNED_BYTE, image->GetData());
    /////////////////

    return true;
}