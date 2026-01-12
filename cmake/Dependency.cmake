include(FetchContent)

# 의존성 리스트 초기화
set(DEP_LIBS "")

# 개별 의존성 파일 경로
set(DEP_CMAKE_DIR "${CMAKE_CURRENT_LIST_DIR}/dependencies")

# 의존성 추가
include(${DEP_CMAKE_DIR}/spdlog.cmake)
include(${DEP_CMAKE_DIR}/glfw.cmake)
include(${DEP_CMAKE_DIR}/glad.cmake)

message(STATUS "Total Dependencies: ${DEP_LIBS}")
