#pragma once

#include "RayTracing/SurfaceMaterial.h"

#include <glm/glm.hpp>

namespace RayMagic {

    // 光线
    struct Ray
    {
        glm::vec3 StartPoint = glm::vec3(0, 0, 0);    // 起点
        glm::vec3 Direction = glm::vec3(0, 0, 0);     // 方向
    };

    // 光线求交结果
    struct HitResult
    {
        bool IsHit = false;                       // 是否命中
        double Distance = 0.0f;                   // 与交点的距离
        glm::vec3 HitPoint = glm::vec3(0, 0, 0);  // 光线命中点
        SurfaceMaterial Material;                 // 命中点的表面材质
    };

}