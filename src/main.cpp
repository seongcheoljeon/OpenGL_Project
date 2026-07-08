#include "context.h"

#include <GLFW/glfw3.h>
#include <glad/glad.h>
#include <spdlog/spdlog.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>


void OnFrameBufferSizeChanged( [[maybe_unused]] GLFWwindow* window, int width, int height )
{
    SPDLOG_INFO("framebuffer size changed: ({} X {})", width, height);
    auto context = static_cast<Context*>(glfwGetWindowUserPointer(window));
    if (context)
    {
        context->Reshape(width, height);
    }
}

void OnKeyEvent( GLFWwindow* window, int key, int scancode, int action, int mods )
{
    ImGui_ImplGlfw_KeyCallback(window, key, scancode, action, mods);
    SPDLOG_INFO("key: {}, scancode: {}, action: {}, mods: {}{}{}", key, scancode,
                action == GLFW_PRESS ? "Pressed"
                : action == GLFW_RELEASE ? "Released"
                : action == GLFW_REPEAT ? "Repeat"
                : "Unknown",
                mods & GLFW_MOD_CONTROL ? "Ctrl" : "-", mods & GLFW_MOD_SHIFT ? "Shift" : "-",
                mods & GLFW_MOD_ALT ? "Alt" : "-");

    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
    {
        glfwSetWindowShouldClose(window, true);
    }
}

void OnCursorPos( GLFWwindow* window, double x, double y )
{
    auto context = static_cast<Context*>(glfwGetWindowUserPointer(window));
    if (context)
    {
        context->MouseMove(x, y);
    }
}

void OnMouseButton( GLFWwindow* window, int button, int action, int modifier )
{
    ImGui_ImplGlfw_MouseButtonCallback(window, button, action, modifier);

    auto context = static_cast<Context*>(glfwGetWindowUserPointer(window));
    double x, y;
    glfwGetCursorPos(window, &x, &y);
    context->MouseButton(button, action, x, y);
}

void OnCharEvent( GLFWwindow* window, unsigned int ch )
{
    ImGui_ImplGlfw_CharCallback(window, ch);
}

void OnScroll( GLFWwindow* window, double xoffset, double yoffset )
{
    ImGui_ImplGlfw_ScrollCallback(window, xoffset, yoffset);
}


int main()
{
    SPDLOG_INFO("Start Program!");

    // glfw 라이브러리 초기화, 실패하면 에러 출력 후 종료.
    SPDLOG_INFO("Initialize glfw");
    if (!glfwInit())
    {
        const char* description = nullptr;
        glfwGetError(&description);
        SPDLOG_ERROR("failed to initialize glfw: {}", description);
        return -1;
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_SCALE_TO_MONITOR, GLFW_TRUE); // 창이 뜨는 모니터 배율에 맞춰 창 크기 자동 조절

    // glfw 윈도우 생성, 실패하면 에러 출력 후 종료.
    SPDLOG_INFO("Create GLFW window");
    auto* window = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, WINDOW_NAME, nullptr, nullptr);
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

    // imgui
    auto imgui_context = ImGui::CreateContext();
    ImGui::SetCurrentContext(imgui_context);

    // 모니터 배율에 맞춰 UI 스케일 적용 (실제 창이 놓인 모니터 기준)
    float xscale = 1.0f;
    float yscale = 1.0f;
    glfwGetWindowContentScale(window, &xscale, &yscale);
    auto& io = ImGui::GetIO();
    io.Fonts->AddFontFromFileTTF(
        "../fonts/NanumSquareNeo-Variable.ttf"
        , 16.0f * xscale
        , nullptr
        , io.Fonts->GetGlyphRangesKorean());
    ImGui::GetStyle().ScaleAllSizes(xscale);

    ImGui_ImplGlfw_InitForOpenGL(window, false);
    ImGui_ImplOpenGL3_Init();
    ImGui_ImplOpenGL3_CreateDeviceObjects();

    //
    ContextUPtr context = Context::Create();
    if (!context)
    {
        SPDLOG_ERROR("failed to create context");
        glfwTerminate();
        return -1;
    }

    glfwSetWindowUserPointer(window, context.get());
    //

    int fb_width = 0;
    int fb_height = 0;
    glfwGetFramebufferSize(window, &fb_width, &fb_height);
    OnFrameBufferSizeChanged(window, fb_width, fb_height);
    glfwSetFramebufferSizeCallback(window, OnFrameBufferSizeChanged);

    glfwSetKeyCallback(window, OnKeyEvent);
    glfwSetCharCallback(window, OnCharEvent);
    glfwSetCursorPosCallback(window, OnCursorPos);
    glfwSetMouseButtonCallback(window, OnMouseButton);
    glfwSetScrollCallback(window, OnScroll);

    // glfw 로프 실행, 윈도우 close 버튼을 누르면 정상 종료.
    SPDLOG_INFO("Start main loop");
    while (!glfwWindowShouldClose(window))
    {
        glfwPollEvents(); // 이벤트 처리

        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        context->ProcessInput(window);
        context->Render();

        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

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
    }
    context.reset();

    ImGui_ImplOpenGL3_DestroyDeviceObjects();
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();

    glfwTerminate();

    return 0;
}