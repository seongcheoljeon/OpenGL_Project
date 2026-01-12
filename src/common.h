//
// Created by seongcheoljeon on 2025-12-08.
//

#ifndef OPENGL_PROJECT_COMMON_H
#define OPENGL_PROJECT_COMMON_H

#include <memory>
#include <string>
#include <optional>
#include <glad/glad.h>
#include <glfw/glfw3.h>
#include <spdlog/spdlog.h>

#define CLASS_PTR(klassName) \
    class klassName; \
    using klassName##UPtr = std::unique_ptr<klassName>; \
    using klassName##SPtr = std::shared_ptr<klassName>; \
    using klassName##WPtr = std::weak_ptr<klassName>;


std::optional<std::string> LoadTextFile(const std::string& filename);

#endif //OPENGL_PROJECT_COMMON_H