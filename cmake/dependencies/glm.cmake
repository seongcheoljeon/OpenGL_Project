message(STATUS "Configuring glm...")

set(GLM_ENABLE_CXX_20 ON CACHE BOOL "Enable C++ 20 features" FORCE)
set(GLM_BUILD_TESTS OFF CACHE BOOL "Build glm tests" FORCE)

FetchContent_Declare(
        glm
        GIT_REPOSITORY "https://github.com/g-truc/glm.git"
        GIT_TAG "1.0.1"
        GIT_SHALLOW TRUE # 가장 최신의 commit만 클론하겠다.
        SYSTEM
)

FetchContent_MakeAvailable(glm)

# glm/ 디렉토리 전체를 external/include로 복사
# 결과: external/include/glm/glm.hpp, glm/vec3.hpp, ...
set(GLM_INCLUDE_DIR "${PROJECT_SOURCE_DIR}/external/include")
file(COPY "${glm_SOURCE_DIR}/glm"
     DESTINATION "${GLM_INCLUDE_DIR}"
)

# include 디렉토리 설정
target_include_directories(${PROJECT_NAME}
        PRIVATE
        "${PROJECT_SOURCE_DIR}/external/include"
)

list(APPEND DEP_LIBS glm::glm)

message(STATUS "Copied glm/ to ${GLM_INCLUDE_DIR}/glm")
