message(STATUS "Configuring spdlog...")

# spdlog 옵션 설정
set(SPDLOG_BUILD_SHARED OFF CACHE BOOL "Build shared library" FORCE)
set(SPDLOG_BUILD_EXAMPLE OFF CACHE BOOL "Build examples" FORCE)
set(SPDLOG_BUILD_TESTS OFF CACHE BOOL "Build tests" FORCE)

FetchContent_Declare(
        spdlog
        GIT_REPOSITORY "https://github.com/gabime/spdlog.git"
        GIT_TAG "v1.x"
        GIT_SHALLOW TRUE # 가장 최신의 commit만 클론하겠다.
        SYSTEM
)

# 사용 가능하게 만들기
FetchContent_MakeAvailable(spdlog)

# 의존성 리스트에 추가
list(APPEND DEP_LIBS spdlog::spdlog)

message(STATUS "spdlog configured successfully.")