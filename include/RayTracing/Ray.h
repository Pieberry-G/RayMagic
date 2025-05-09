#pragma once

#include "RayTracing/Material.h"

#include <glm/glm.hpp>

namespace RayMagic {

    // 光线
    struct Ray
    {
        glm::vec3 startPoint = glm::vec3(0, 0, 0);    // 起点
        glm::vec3 direction = glm::vec3(0, 0, 0);     // 方向
    };

    // 光线求交结果
    struct HitResult
    {
        float distance = FLT_MAX;                 // 与交点的距离
        glm::vec3 hitPoint = glm::vec3(0, 0, 0);  // 光线命中点
        Material material;                 // 命中点的表面材质

        bool IsHit() { return distance < FLT_MAX; }
    };

}