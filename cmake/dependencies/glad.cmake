message(STATUS "Configuring GLAD...")

set(GLAD_DIR "${CMAKE_SOURCE_DIR}/external/glad")

if(NOT EXISTS "${GLAD_DIR}/src/glad.c")
    message(FATAL_ERROR "GLAD not found. Download from https://glad.dav1d.de/")
endif()

add_library(glad STATIC "${GLAD_DIR}/src/glad.c")
target_include_directories(glad PUBLIC "${GLAD_DIR}/include")

list(APPEND DEP_LIBS glad)

message(STATUS "GLAD configured successfully.")
