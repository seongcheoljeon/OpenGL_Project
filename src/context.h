//
// Created by seongcheoljeon on 2025-12-22.
//

#ifndef OPENGL_PROJECT_CONTEXT_H
#define OPENGL_PROJECT_CONTEXT_H

#include "common.h"
#include "shader.h"
#include "program.h"
#include "buffer.h"
#include "mesh.h"
#include "model.h"
#include "vertex_layout.h"
#include "texture.h"


CLASS_PTR(Context)

class Context
{
public:
    static ContextUPtr Create();

    void Render();

    void ProcessInput( GLFWwindow* window );

    void Reshape( int width, int height );

    void MouseMove( double x, double y );

    void MouseButton( int button, int action, double x, double y );

private:
    Context() = default;

    bool _Init();

private:
    ProgramUPtr _program{nullptr};
    ProgramUPtr _simple_program{nullptr};
    ProgramUPtr _texture_program{nullptr};

    MeshUPtr _box{nullptr};
    MeshUPtr _plane{nullptr};

    // clear color
    glm::vec4 _clear_color{glm::vec4(0.1f, 0.2f, 0.3f, 0.0f)};

private:
    int _width{WINDOW_WIDTH};
    int _height{WINDOW_HEIGHT};

    // camera parameter
private:
    glm::vec3 _camera_pos{glm::vec3(0.0f, 2.5f, 8.0f)};
    glm::vec3 _camera_front{glm::vec3(0.0f, 0.0f, -1.0f)};
    glm::vec3 _camera_up{glm::vec3(0.0f, 1.0f, 0.0f)};
    glm::vec2 _prev_mouse_pos{glm::vec2(0.0f)};

    float _camera_speed{0.05f};
    float _camera_pitch{-20.0f};
    float _camera_yaw{0.0f};
    bool _is_camera_control{false};

    // light parameter
private:
    struct Light
    {
        glm::vec3 position{glm::vec3(1.0f, 4.0f, 4.0f)};
        glm::vec3 direction{glm::vec3(-1.0f, -1.0f, -1.0f)};
        glm::vec2 cutoff{glm::vec2(120.0f, 5.0f)};
        float distance{128.0f};
        glm::vec3 ambient{glm::vec3(0.1f, 0.1f, 0.1f)};
        glm::vec3 diffuse{glm::vec3(0.8f, 0.8f, 0.8f)};
        glm::vec3 specular{glm::vec3(1.0f, 1.0f, 1.0f)};
    };

    Light _light;

    // material parameter
private:
    MaterialSPtr _plane_material;
    MaterialSPtr _box1_material;
    MaterialSPtr _box2_material;
    TextureSPtr _window_texture;

private:
    bool _is_animation{true};
    bool _is_flash_light_mode{false};
};

#endif // OPENGL_PROJECT_CONTEXT_H