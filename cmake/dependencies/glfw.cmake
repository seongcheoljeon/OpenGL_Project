message(STATUS "Configuring GLFW...")

set(GLFW_BUILD_DOCS OFF CACHE BOOL "Build GLFW documentation" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "Build GLFW tests" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "Build GLFW examples" FORCE)

FetchContent_Declare(
        glfw
        GIT_REPOSITORY "https://github.com/glfw/glfw.git"
        GIT_TAG "3.3.10"
        GIT_SHALLOW TRUE # 가장 최신의 commit만 클론하겠다.
        SYSTEM
)

FetchContent_MakeAvailable(glfw)

list(APPEND DEP_LIBS glfw)

message(STATUS "GLFW configured successfully.")