//
// Created by seongcheoljeon on 2025-12-08.
//

#include "common.h"
#include <fstream>
#include <sstream>


std::optional<std::string> LoadTextFile(const std::string &filename)
{
    std::ifstream fin(filename);
    if (!fin.is_open())
    {
        SPDLOG_ERROR("Can't open file {}", filename);
        return {};
    }
    std::stringstream text;
    text << fin.rdbuf();
    return text.str();
}

glm::vec3 GetAttenuationCoefficients( float distance )
{
    constexpr auto linear_coeff = glm::vec4(
        8.4523112e-05, 4.4712582e+00, -1.8516388e+00, 3.3955811e+01);
    constexpr auto quad_coeff = glm::vec4(
        -7.6103583e-04, 9.0120201e+00, -1.1618500e+01, 1.0000464e+02);

    const float kc = 1.0f;
    const float d = 1.0f / distance;
    const glm::vec4 dvec = glm::vec4(1.0f, d, d * d, d * d * d);
    const float kl = glm::dot(linear_coeff, dvec);
    const float kq = glm::dot(quad_coeff, dvec);

    return glm::vec3(kc, glm::max(kl, 0.0f), glm::max(kq * kq, 0.0f));
}