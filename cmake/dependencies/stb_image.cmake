message(STATUS "Configuring stb_image...")

FetchContent_Declare(
        stb_image
        GIT_REPOSITORY "https://github.com/nothings/stb.git"
        GIT_TAG "master"
        GIT_SHALLOW TRUE # 가장 최신의 commit만 클론하겠다.
        SYSTEM
)

FetchContent_MakeAvailable(stb_image)

# stb_image.h를 external/include/stb로 복사
set(STB_INCLUDE_DIR "${PROJECT_SOURCE_DIR}/external/include/stb")
file(COPY "${stb_image_SOURCE_DIR}/stb_image.h"
     DESTINATION "${STB_INCLUDE_DIR}"
)

# include 디렉토리 설정
target_include_directories(${PROJECT_NAME}
        PRIVATE
        "${PROJECT_SOURCE_DIR}/external/include"
)

message(STATUS "Copied stb_image.h to ${STB_INCLUDE_DIR}")
