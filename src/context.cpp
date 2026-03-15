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
    std::vector<glm::vec3> cube_positions = {
        glm::vec3(0.0f, 0.0f, 0.0f),
        glm::vec3(2.0f, 5.0f, -15.0f),
        glm::vec3(-1.5f, -2.2f, -2.5f),
        glm::vec3(-3.8f, -2.0f, -12.3f),
        glm::vec3(2.4f, -0.4f, -3.5f),
        glm::vec3(-1.7f, 3.0f, -7.5f),
        glm::vec3(1.3f, -2.0f, -2.5f),
        glm::vec3(1.5f, 2.0f, -2.5f),
        glm::vec3(1.5f, 0.2f, -1.5f),
        glm::vec3(-1.3f, 1.0f, -1.5f)
    };

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glEnable(GL_DEPTH_TEST);

    _camera_front =
        glm::rotate(glm::mat4(1.0f), glm::radians(_camera_yaw), glm::vec3(0.0f, 1.0f, 0.0f))
        * glm::rotate(glm::mat4(1.0f), glm::radians(_camera_pitch), glm::vec3(1.0f, 0.0f, 0.0f))
        * glm::vec4(0.0f, 0.0f, -1.0f, 0.0f); // 방향 vector일 경우 w값에 0.0을 입력해야 하는 필요가 있을 수 있다.

    auto projection = glm::perspective(glm::radians(45.0f)
        , static_cast<float>(_width) / static_cast<float>(_height), 0.01f, 100.f);

    auto view = glm::lookAt(_camera_pos
        , _camera_pos + _camera_front
        , _camera_up);

    for (size_t i=0; i<cube_positions.size(); ++i)
    {
        auto& pos = cube_positions[i];
        auto model = glm::translate(glm::mat4(1.0f), pos);
        model = glm::rotate(model
            , glm::radians((float)glfwGetTime() * 120.0f + 20.0f  * (float)i)
            , glm::vec3(1.0f, 0.5f, 0.0f));
        auto transform = projection * view * model;
        _program->SetUniform("transform", transform);
        glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
    }

    _program->Use();
}

void Context::ProcessInput( GLFWwindow* window )
{
    if (false == _is_camera_control)
    {
        return;
    }

    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
    {
        _camera_pos += _camera_speed * _camera_front;
    }
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
    {
        _camera_pos -= _camera_speed * _camera_front;
    }

    auto camera_right = glm::normalize(glm::cross(_camera_up, -_camera_front));
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
    {
        _camera_pos += _camera_speed * camera_right;
    }
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
    {
        _camera_pos -= _camera_speed * camera_right;
    }

    auto camera_up = glm::normalize(glm::cross(-_camera_front, camera_right));
    if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS)
    {
        _camera_pos += _camera_speed * camera_up;
    }
    if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS)
    {
        _camera_pos -= _camera_speed * camera_up;
    }
}

void Context::Reshape( int width, int height )
{
    _width = width;
    _height = height;
    glViewport(0, 0, _width, _height); // opengl이 그림을 그릴 화면의 위치 및 크기 설정
}

void Context::MouseMove( double x, double y )
{
    if (false == _is_camera_control)
    {
        return;
    }
    auto pos = glm::vec2(static_cast<float>(x), static_cast<float>(y));
    auto delta_pos = pos - _prev_mouse_pos;

    const float camera_rot_speed = 0.1f;

    _camera_yaw -= delta_pos.x * camera_rot_speed;
    _camera_pitch -= delta_pos.y * camera_rot_speed;

    if (_camera_yaw < 0.0f) _camera_yaw += 360.0f;
    if (_camera_yaw > 360.0f) _camera_yaw -= 360.0f;

    if (_camera_pitch > 89.0f) _camera_pitch = 89.0f;
    if (_camera_pitch < -89.0f) _camera_pitch = -89.0f;

    _prev_mouse_pos = pos;
}

void Context::MouseButton( int button, int action, double x, double y )
{
    if (GLFW_MOUSE_BUTTON_RIGHT == button)
    {
        if (GLFW_PRESS == action)
        {
            _prev_mouse_pos = glm::vec2(static_cast<float>(x), static_cast<float>(y));
            _is_camera_control = true;
        }
        else if (GLFW_RELEASE == action)
        {
            _is_camera_control = false;
        }
    }
}

bool Context::_Init()
{
    float vertices[] = {
        -0.5f, -0.5f, -0.5f, 0.0f, 0.0f
        , 0.5f, -0.5f, -0.5f, 1.0f, 0.0f
        , 0.5f,  0.5f, -0.5f, 1.0f, 1.0f
        , -0.5f, 0.5f, -0.5f, 0.0f, 1.0f

        , -0.5f, -0.5f, 0.5f, 0.0f, 0.0f
        , 0.5f, -0.5f, 0.5f, 1.0f, 0.0f
        , 0.5f , 0.5f, 0.5f, 1.0f, 1.0f
        , -0.5f, 0.5f, 0.5f, 0.0f, 1.0f

        , -0.5f, 0.5f, 0.5f, 1.0f, 0.0f
        , -0.5f, 0.5f, -0.5f, 1.0f, 1.0f
        , -0.5f, -0.5f, -0.5f, 0.0f, 1.0f
        , -0.5f, -0.5f, 0.5f, 0.0f, 0.0f

        , 0.5f, 0.5f, 0.5f, 1.0f, 0.0f
        , 0.5f, 0.5f, -0.5f, 1.0f, 1.0f
        , 0.5f, -0.5f, -0.5f, 0.0f, 1.0f
        , 0.5f, -0.5f, 0.5f, 0.0f, 0.0f

        , -0.5f, -0.5f, -0.5f, 0.0f, 1.0f
        , 0.5f, -0.5f, -0.5f, 1.0f, 1.0f
        , 0.5f, -0.5f, 0.5f, 1.0f, 0.0f
        , -0.5f, -0.5f, 0.5f, 0.0f, 0.0f

        , -0.5f, 0.5f, -0.5f, 0.0f, 1.0f
        , 0.5f, 0.5f, -0.5f, 1.0f, 1.0f
        , 0.5f, 0.5f, 0.5f, 1.0f, 0.0f
        , -0.5f, 0.5f, 0.5f, 0.0f, 0.0f
    };

    uint32_t indices[] = {
        0, 2, 1, 2, 0, 3
        , 4, 5, 6, 6, 7, 4
        , 8, 9, 10, 10, 11, 8
        , 12, 14, 13, 14, 12, 15
        , 16, 17, 18, 18, 19, 16
        , 20, 22, 21, 22, 20, 23
    };

    // 순서 중요!
    // VAO binding -> VBO binding -> vertex attribute 설정

    _vertex_layout = VertexLayout::Create();

    // vertex buffer 생성 (position, normal, 등등이 들어가는 버퍼)
    _vertex_buffer = Buffer::CreateWithData(GL_ARRAY_BUFFER, GL_STATIC_DRAW
        , vertices, sizeof(float) * 120);

    // _vertex_layout->SetAttrib(0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 3, 0);
    _vertex_layout->SetAttrib(0, 3, GL_FLOAT, GL_FALSE
        , sizeof(float) * 5, 0);
    _vertex_layout->SetAttrib(2, 2, GL_FLOAT, GL_FALSE
        , sizeof(float) * 5, sizeof(float) * 3);

    // index buffer 생성 및 바인딩
    _index_buffer = Buffer::CreateWithData(GL_ELEMENT_ARRAY_BUFFER, GL_STATIC_DRAW
        , indices, sizeof(uint32_t) * 36);

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

    return true;
}