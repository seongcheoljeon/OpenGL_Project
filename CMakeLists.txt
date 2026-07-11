cmake_minimum_required(VERSION 4.0)

set(PROJECT_NAME "OpenGL_Project")

project(${PROJECT_NAME}
        VERSION 1.0.0
        LANGUAGES C CXX
)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

##### 윈도우 설정 #####
set(WINDOW_NAME "SeongcheolJeon's OpenGL")
set(WINDOW_WIDTH 1280)
set(WINDOW_HEIGHT 720)

add_executable(${PROJECT_NAME}
        src/common.h
        src/common.cpp
        src/shader.cpp
        src/shader.h
        src/program.cpp
        src/program.h
        src/context.cpp
        src/context.h
        src/buffer.h
        src/buffer.cpp
        src/vertex_layout.cpp
        src/vertex_layout.h
        src/image.cpp
        src/image.h
        src/texture.cpp
        src/texture.h
        src/mesh.h
        src/mesh.cpp
        src/model.cpp
        src/model.h
)

target_sources(${PROJECT_NAME}
        PRIVATE
        src/main.cpp
)

# cmake 모듈 경로 추가
list(APPEND CMAKE_MODULE_PATH "${PROJECT_SOURCE_DIR}/cmake")

# 의존성 포함
include(Dependency)

# 의존성 링크
target_link_libraries(${PROJECT_NAME}
        PRIVATE
        ${DEP_LIBS}
)

# 컴파일 가능 요구사항
target_compile_features(${PROJECT_NAME}
        PRIVATE
        cxx_std_20
)

# 빌드 타입별 설정 (멀티-config 생성기 지원을 위해 generator expression 사용)
target_compile_definitions(${PROJECT_NAME}
        PRIVATE
        $<$<CONFIG:Debug>:DEBUG_BUILD>
        PUBLIC
        WINDOW_NAME="${WINDOW_NAME}$<$<CONFIG:Debug>: [Debug]>"
        WINDOW_WIDTH=${WINDOW_WIDTH}
        WINDOW_HEIGHT=${WINDOW_HEIGHT}
)

# 컴파일 경고 설정
if(MSVC)
    target_compile_options(${PROJECT_NAME}
            PRIVATE
            /W4
    )
else()
    target_compile_options(${PROJECT_NAME}
            PRIVATE
            -Wall
            -Wextra
            -Wpedantic
    )
endif()
