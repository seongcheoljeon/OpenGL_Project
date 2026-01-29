message(STATUS "Configuring stb_image...")

FetchContent_Declare(
        stb_image
        GIT_REPOSITORY "https://github.com/nothings/stb.git"
        GIT_TAG "master"
        GIT_SHALLOW TRUE # 가장 최신의 commit만 클론하겠다.
        SYSTEM
)

FetchContent_MakeAvailable(stb_image)

# stb_image.h를 빌드 디렉토리의 include/stb로 복사
file(COPY "${stb_image_SOURCE_DIR}/stb_image.h"
     DESTINATION "${CMAKE_BINARY_DIR}/include/stb"
)

message(STATUS "Copied stb_image.h to ${CMAKE_BINARY_DIR}/include/stb")
