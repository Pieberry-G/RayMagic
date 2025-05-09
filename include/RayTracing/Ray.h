#pragma once

#include "RayTracing/Material.h"

#include <glm/glm.hpp>

namespace RayMagic {

    // ����
    struct Ray
    {
        glm::vec3 startPoint = glm::vec3(0, 0, 0);    // ���
        glm::vec3 direction = glm::vec3(0, 0, 0);     // ����
    };

    // �����󽻽��
    struct HitResult
    {
        float distance = FLT_MAX;                 // �뽻��ľ���
        glm::vec3 hitPoint = glm::vec3(0, 0, 0);  // �������е�
        Material material;                 // ���е�ı������

        bool IsHit() { return distance < FLT_MAX; }
    };

}