#pragma once

#include <glm/glm.hpp>

namespace RayMagic {

    // 物体表面材质定义
    struct Material
    {
        bool isEmissive = false;               // 是否发光
        glm::vec3 normal = glm::vec3(0, 0, 0); // 法向量
        glm::vec3 color = glm::vec3(0, 0, 0);  // 颜色
        double specularRate = 0.0f;            // 反射光占比
        double roughness = 1.0f;               // 粗糙程度
        double refractRate = 0.0f;             // 折射光占比
        double refractAngle = 1.0f;            // 折射率
        double refractRoughness = 0.0f;        // 折射粗糙度
    };

}