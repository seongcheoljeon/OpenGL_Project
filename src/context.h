//
// Created by seongcheoljeon on 2025-12-22.
//

#ifndef OPENGL_PROJECT_CONTEXT_H
#define OPENGL_PROJECT_CONTEXT_H

#include "common.h"
#include "shader.h"
#include "program.h"
#include "buffer.h"
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

    void MouseMove(double x, double y);
    void MouseButton(int button, int action, double x, double y);

private:
    Context() = default;

    bool _Init();

private:
    ProgramUPtr _program{nullptr};

    VertexLayoutUPtr _vertex_layout{nullptr};
    BufferUPtr _vertex_buffer{nullptr};
    BufferUPtr _index_buffer{nullptr};
    TextureUPtr _texture{nullptr};
    TextureUPtr _texture2{nullptr};

private:
    int _width{WINDOW_HEIGHT};
    int _height{WINDOW_HEIGHT};

    // camera parameter
private:
    glm::vec3 _camera_pos{glm::vec3(0.0f, 0.0f, 3.0f)};
    glm::vec3 _camera_front{glm::vec3(0.0f, 0.0f, -1.0f)};
    glm::vec3 _camera_up{glm::vec3(0.0f, 1.0f, 0.0f)};
    glm::vec2 _prev_mouse_pos{glm::vec2(0.0f)};

    float _camera_speed{0.05f};
    float _camera_pitch{0.0f};
    float _camera_yaw{0.0f};
    bool _is_camera_control{false};
};

#endif // OPENGL_PROJECT_CONTEXT_H