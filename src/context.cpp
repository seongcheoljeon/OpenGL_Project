//
// Created by seongcheoljeon on 2025-12-22.
//

#include "context.h"
#include "image.h"
#include "imgui.h"

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
    if (ImGui::Begin("UI Window"))
    {
        if (ImGui::ColorEdit4("Clear Color", glm::value_ptr(_clear_color)))
        {
            glClearColor(_clear_color.x, _clear_color.y, _clear_color.z, _clear_color.w);
        }
        ImGui::Separator();
        ImGui::DragFloat3("Camera Position", glm::value_ptr(_camera_pos), 0.01f);
        ImGui::DragFloat("Camera Yaw", &_camera_yaw, 0.5f);
        ImGui::DragFloat("Camera Pitch", &_camera_pitch, 0.5f, -89.0f, 89.0f);
        ImGui::Separator();
        if (ImGui::Button("Reset Camera"))
        {
            _camera_yaw   = 0.0f;
            _camera_pitch = 0.0f;
            _camera_pos   = glm::vec3(0.0f, 0.0f, 3.0f);
        }

        if (ImGui::CollapsingHeader("Light", ImGuiTreeNodeFlags_DefaultOpen))
        {
            ImGui::DragFloat3("l.Position", glm::value_ptr(_light.position), 0.01f);
            ImGui::DragFloat3("l.Direction", glm::value_ptr(_light.direction), 0.01f);
            ImGui::DragFloat2("l.Cutoff", glm::value_ptr(_light.cutoff), 0.5f, 0.0f, 90.0f);
            ImGui::DragFloat("l.Distance", &_light.distance, 0.5f, 0.0f, 3000.0f);
            ImGui::ColorEdit3("l.Ambient", glm::value_ptr(_light.ambient));
            ImGui::ColorEdit3("l.Diffuse", glm::value_ptr(_light.diffuse));
            ImGui::ColorEdit3("l.Specular", glm::value_ptr(_light.specular));
            ImGui::Checkbox("l.Flash Light", &_is_flash_light_mode);
        }

        if (ImGui::CollapsingHeader("Material", ImGuiTreeNodeFlags_DefaultOpen))
        {
            ImGui::DragFloat("m.shininess", &_material.shininess, 1.0f, 1.0f, 256.0f);
        }

        ImGui::Checkbox("Animation", &_is_animation);
    }
    ImGui::End();

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

    glm::vec3 light_pos = _light.position;
    glm::vec3 light_dir = _light.direction;

    if (_is_flash_light_mode)
    {
        light_pos = _camera_pos;
        light_dir = _camera_front;
    }
    else
    {
        auto light_model_transform = glm::translate(glm::mat4(1.0), _light.position)
                                     * glm::scale(glm::mat4(1.0), glm::vec3(0.1f));
        _simple_program->Use();
        _simple_program->SetUniform("color", glm::vec4(_light.ambient + _light.diffuse, 1.0f));
        _simple_program->SetUniform("transform", projection * view * light_model_transform);
        _box->Draw(_simple_program.get());
    }

    _program->Use();
    _program->SetUniform("view_pos", _camera_pos);
    _program->SetUniform("light.position", light_pos);
    _program->SetUniform("light.direction", light_dir);
    _program->SetUniform("light.cutoff"
                         , glm::vec2(cosf(glm::radians(_light.cutoff[0]))
                                     , cosf(glm::radians(_light.cutoff[0] + _light.cutoff[1]))));
    _program->SetUniform("light.attenuation", GetAttenuationCoefficients(_light.distance));
    _program->SetUniform("light.ambient", _light.ambient);
    _program->SetUniform("light.diffuse", _light.diffuse);
    _program->SetUniform("light.specular", _light.specular);

    _program->SetUniform("material.diffuse", 0);
    _program->SetUniform("material.specular", 1);
    _program->SetUniform("material.shininess", _material.shininess);

    glActiveTexture(GL_TEXTURE0);
    _material.diffuse->Bind();
    glActiveTexture(GL_TEXTURE1);
    _material.specular->Bind();

    auto model_transform = glm::mat4(1.0f);
    auto transform = projection * view * model_transform;
    _program->SetUniform("transform", transform);
    _program->SetUniform("model_transform", model_transform);
    _model->Draw(_program.get());
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
    _width  = width;
    _height = height;
    glViewport(0, 0, _width, _height); // opengl이 그림을 그릴 화면의 위치 및 크기 설정
}

void Context::MouseMove( double x, double y )
{
    if (false == _is_camera_control)
    {
        return;
    }
    auto pos       = glm::vec2(static_cast<float>(x), static_cast<float>(y));
    auto delta_pos = pos - _prev_mouse_pos;

    const float camera_rot_speed = 0.1f;

    _camera_yaw   -= delta_pos.x * camera_rot_speed;
    _camera_pitch -= delta_pos.y * camera_rot_speed;

    if (_camera_yaw < 0.0f)
        _camera_yaw += 360.0f;
    if (_camera_yaw > 360.0f)
        _camera_yaw -= 360.0f;

    if (_camera_pitch > 89.0f)
        _camera_pitch = 89.0f;
    if (_camera_pitch < -89.0f)
        _camera_pitch = -89.0f;

    _prev_mouse_pos = pos;
}

void Context::MouseButton( int button, int action, double x, double y )
{
    if (GLFW_MOUSE_BUTTON_RIGHT == button)
    {
        if (GLFW_PRESS == action)
        {
            _prev_mouse_pos    = glm::vec2(static_cast<float>(x), static_cast<float>(y));
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
    _box = Mesh::CreateBox();

    _model = Model::Load("../model/backpack.obj");
    if (_model == nullptr)
    {
        return false;
    }

    _simple_program = Program::Create("../shader/simple.vert", "../shader/simple.frag");
    if (!_simple_program)
    {
        return false;
    }

    _program = Program::Create("../shader/lighting.vert", "../shader/lighting.frag");
    if (!_program)
    {
        return false;
    }

    glClearColor(0.0f, 0.1f, 0.2f, 0.0f); // color framebuffer 화면을 클리어

    // auto diffuse_image = Image::Load("../image/container2.png");
    auto diffuse_image = Image::CreateSingleColor(4, 4, glm::vec4(1.0f, 1.0f, 1.0f, 1.0f));
    if (!diffuse_image)
    {
        return false;
    }
    SPDLOG_INFO("diffuse image: {}x{}, {} channels"
                , diffuse_image->GetWidth(), diffuse_image->GetHeight(), diffuse_image->GetChannelCount());
    _material.diffuse = Texture::CreateFromImage(diffuse_image.get());

    // auto specular_image = Image::Load("../image/container2_specular.png");
    auto specular_image = Image::CreateSingleColor(4, 4, glm::vec4(0.5f, 0.5f, 0.5f, 1.0f));
    if (!specular_image)
    {
        return false;
    }
    SPDLOG_INFO("specular image: {}x{}, {} channels"
                , specular_image->GetWidth(), specular_image->GetHeight(), specular_image->GetChannelCount());
    _material.specular = Texture::CreateFromImage(specular_image.get());

    return true;
}