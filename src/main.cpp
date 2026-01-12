#include "context.h"

#include <GLFW/glfw3.h>
#include <glad/glad.h>
#include <spdlog/spdlog.h>

void OnFrameBufferSizeChanged(GLFWwindow *window, int width, int height)
{
    SPDLOG_INFO("framebuffer size changed: ({} X {})", width, height);
    glViewport(0, 0, width, height); // opengl이 그림을 그릴 화면의 위치 및 크기 설정
}

void OnKeyEvent(GLFWwindow *window, int key, int scancode, int action, int mods)
{
    SPDLOG_INFO("key: {}, scancode: {}, action: {}, mods: {}{}{}", key, scancode,
                action == GLFW_PRESS     ? "Pressed"
                : action == GLFW_RELEASE ? "Released"
                : action == GLFW_REPEAT  ? "Repeat"
                                         : "Unknown",
                mods & GLFW_MOD_CONTROL ? "Ctrl" : "-", mods & GLFW_MOD_SHIFT ? "Shift" : "-",
                mods & GLFW_MOD_ALT ? "Alt" : "-");

    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
    {
        glfwSetWindowShouldClose(window, true);
    }
}

int main()
{
    SPDLOG_INFO("Start Program!");

    // glfw 라이브러리 초기화, 실패하면 에러 출력 후 종료.
    SPDLOG_INFO("Initialize glfw");
    if (!glfwInit())
    {
        const char *description = nullptr;
        glfwGetError(&description);
        SPDLOG_ERROR("failed to initialize glfw: {}", description);
        return -1;
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    // glfw 윈도우 생성, 실패하면 에러 출력 후 종료.
    SPDLOG_INFO("Create GLFW window");
    auto *window = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, WINDOW_NAME, nullptr, nullptr);
    if (!window)
    {
        SPDLOG_ERROR("failed to create glfw window");
        glfwTerminate();
        return -1;
    }

    // opengl context 설정
    glfwMakeContextCurrent(window);

    // glad를 활용한 opengl 함수 로딩
    if (!gladLoadGLLoader(reinterpret_cast<GLADloadproc>(glfwGetProcAddress)))
    {
        SPDLOG_ERROR("failed to initialize glfw OpenGL context");
        glfwTerminate();
        return -1;
    }

    auto glVersion = glGetString(GL_VERSION);
    if (glVersion)
    {
        SPDLOG_INFO("OpenGL context version: {}", reinterpret_cast<const char *>(glVersion));
    }
    else
    {
        SPDLOG_WARN("glGetString(GL_VERSION) returned null");
    }

    //
    ContextUPtr context = Context::Create();
    if (!context)
    {
        SPDLOG_ERROR("failed to create context");
        glfwTerminate();
        return -1;
    }
    //

    OnFrameBufferSizeChanged(window, WINDOW_WIDTH, WINDOW_HEIGHT);
    glfwSetFramebufferSizeCallback(window, OnFrameBufferSizeChanged);
    glfwSetKeyCallback(window, OnKeyEvent);

    // glfw 로프 실행, 윈도우 close 버튼을 누르면 정상 종료.
    SPDLOG_INFO("Start main loop");
    while (!glfwWindowShouldClose(window))
    {
        context->Render();
        /*
         * 화면에 그림을 그리는 과정
         * ...프레임 버퍼 2개를 준비 (front / back)
         * ...back buffer에 그림 그리기
         * ...front와 back을 바꿔치기
         * ...위의 과정을 반복
         * 그림이 그려지는 과정이 노출되지 않도록 해줌
         * 이것을 더블 버퍼링(Double Buffering)이라고 부름
         */
        glfwSwapBuffers(window);
        glfwPollEvents();                     // 이벤트 처리
    }
    context.reset();

    glfwTerminate();

    return 0;
}