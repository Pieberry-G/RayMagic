#pragma once

#include "RayTracing/SurfaceMaterial.h"

#include <glm/glm.hpp>

namespace RayMagic {

    // ����
    struct Ray
    {
        glm::vec3 StartPoint = glm::vec3(0, 0, 0);    // ���
        glm::vec3 Direction = glm::vec3(0, 0, 0);     // ����
    };

    // �����󽻽��
    struct HitResult
    {
        bool IsHit = false;                       // �Ƿ�����
        double Distance = 0.0f;                   // �뽻��ľ���
        glm::vec3 HitPoint = glm::vec3(0, 0, 0);  // �������е�
        SurfaceMaterial Material;                 // ���е�ı������
    };

}