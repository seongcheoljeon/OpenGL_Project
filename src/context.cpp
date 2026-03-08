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
    return context;
}

void Context::Render()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glEnable(GL_DEPTH_TEST);

    _program->Use();

    auto model = glm::rotate(glm::mat4(1.0f), glm::radians((float)glfwGetTime() * 120.0f), glm::vec3(1.0f, 0.f, 0.f));
    auto view = glm::translate(glm::mat4(1.f), glm::vec3(0.f, 0.f, -3.f));
    auto projection = glm::perspective(glm::radians(45.f), (float)WINDOW_WIDTH/(float)WINDOW_HEIGHT, 0.01f, 10.f);
    auto transform = projection * view * model;
    _program->SetUniform("transform", transform);

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

    ShaderSPtr vertex_shader   = Shader::CreateFromFile("../shader/texture.vert", GL_VERTEX_SHADER);
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

    _texture = Texture::CreateFromImage(image.get());

    auto image2 = Image::Load("../image/awesomeface.png");
    if (!image2)
    {
        return false;
    }
    _texture2 = Texture::CreateFromImage(image2.get());

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, _texture->Get());
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, _texture2->Get());

    _program->Use();
    _program->SetUniform("tex", 0);
    _program->SetUniform("tex2", 1);

    auto model = glm::rotate(glm::mat4(1.0f), glm::radians(-55.f), glm::vec3(1.0f, 0.f, 0.f));
    auto view = glm::translate(glm::mat4(1.f), glm::vec3(0.f, 0.f, -3.f));
    auto projection = glm::perspective(glm::radians(45.f), (float)WINDOW_WIDTH/(float)WINDOW_HEIGHT, 0.01f, 10.f);
    auto transform = projection * view * model;
    _program->SetUniform("transform", transform);

    return true;
}