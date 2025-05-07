#pragma once

#include <glm/glm.hpp>

namespace RayMagic {

    // 物体表面材质定义
    struct SurfaceMaterial
    {
        bool IsEmissive = false;               // 是否发光
        glm::vec3 Normal = glm::vec3(0, 0, 0); // 法向量
        glm::vec3 Color = glm::vec3(0, 0, 0);  // 颜色
        double SpecularRate = 0.0f;            // 反射光占比
        double Roughness = 1.0f;               // 粗糙程度
        double RefractRate = 0.0f;             // 折射光占比
        double RefractAngle = 1.0f;            // 折射率
        double RefractRoughness = 0.0f;        // 折射粗糙度
    };

}