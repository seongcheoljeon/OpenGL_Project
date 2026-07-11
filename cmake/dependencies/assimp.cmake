message(STATUS "Configuring assimp...")

# assimp 옵션 설정
set(BUILD_SHARED_LIBS OFF CACHE BOOL "Build shared library" FORCE)
set(ASSIMP_BUILD_TESTS OFF CACHE BOOL "Build assimp tests" FORCE)
set(ASSIMP_BUILD_ASSIMP_TOOLS OFF CACHE BOOL "Build assimp tools" FORCE)
set(ASSIMP_INSTALL OFF CACHE BOOL "Install assimp" FORCE)
set(ASSIMP_BUILD_ZLIB ON CACHE BOOL "Build zlib" FORCE)
set(ASSIMP_INJECT_DEBUG_POSTFIX OFF CACHE BOOL "Inject debug postfix" FORCE)
set(ASSIMP_WARNINGS_AS_ERRORS OFF CACHE BOOL "Treat warnings as errors" FORCE)

FetchContent_Declare(
        assimp
        GIT_REPOSITORY "https://github.com/assimp/assimp.git"
        GIT_TAG "v6.0.5"
        GIT_SHALLOW TRUE # 가장 최신의 commit만 클론하겠다.
        SYSTEM
)

FetchContent_MakeAvailable(assimp)

list(APPEND DEP_LIBS assimp::assimp)

message(STATUS "assimp configured successfully.")