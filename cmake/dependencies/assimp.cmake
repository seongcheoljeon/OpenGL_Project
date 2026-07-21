message(STATUS "Configuring assimp...")

# assimp 옵션 설정
set(BUILD_SHARED_LIBS OFF CACHE BOOL "Build shared library" FORCE)
set(ASSIMP_BUILD_TESTS OFF CACHE BOOL "Build assimp tests" FORCE)
set(ASSIMP_BUILD_ASSIMP_TOOLS OFF CACHE BOOL "Build assimp tools" FORCE)
set(ASSIMP_INSTALL OFF CACHE BOOL "Install assimp" FORCE)
# zlib 선택:
#  - macOS: 번들 zlib(구버전)의 zutil.h가 TARGET_OS_MAC 감지 후 fdopen 매크로를 NULL로 정의해
#           최신 clang의 <stdio.h> fdopen 선언과 충돌 -> 빌드 실패. macOS SDK의 시스템 zlib 사용.
#  - Windows/기타: 시스템 zlib을 보장할 수 없고 번들 zlib 버그도 안 터지므로 번들 zlib 사용.
if(APPLE)
    set(ASSIMP_BUILD_ZLIB OFF CACHE BOOL "Build zlib" FORCE)
else()
    set(ASSIMP_BUILD_ZLIB ON CACHE BOOL "Build zlib" FORCE)
endif()
set(ASSIMP_INJECT_DEBUG_POSTFIX OFF CACHE BOOL "Inject debug postfix" FORCE)
set(ASSIMP_WARNINGS_AS_ERRORS OFF CACHE BOOL "Treat warnings as errors" FORCE)

##################################################################################################
# 빌드 시간 단축: 필요한 임포터만 빌드하고 익스포터는 제외
set(ASSIMP_NO_EXPORT ON CACHE BOOL "Disable export API" FORCE)
set(ASSIMP_BUILD_ALL_IMPORTERS_BY_DEFAULT OFF CACHE BOOL "Disable all importers by default" FORCE)
set(ASSIMP_BUILD_OBJ_IMPORTER ON CACHE BOOL "Build OBJ importer" FORCE)
set(ASSIMP_BUILD_FBX_IMPORTER ON CACHE BOOL "Build FBX importer" FORCE)
set(ASSIMP_BUILD_GLTF_IMPORTER ON CACHE BOOL "Build glTF importer" FORCE)
#set(ASSIMP_BUILD_BLEND_IMPORTER ON CACHE BOOL "Build Blender importer" FORCE)
##################################################################################################

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