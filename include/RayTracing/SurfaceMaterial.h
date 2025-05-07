#pragma once

#include <glm/glm.hpp>

namespace RayMagic {

    // ���������ʶ���
    struct SurfaceMaterial
    {
        bool IsEmissive = false;               // �Ƿ񷢹�
        glm::vec3 Normal = glm::vec3(0, 0, 0); // ������
        glm::vec3 Color = glm::vec3(0, 0, 0);  // ��ɫ
        double SpecularRate = 0.0f;            // �����ռ��
        double Roughness = 1.0f;               // �ֲڳ̶�
        double RefractRate = 0.0f;             // �����ռ��
        double RefractAngle = 1.0f;            // ������
        double RefractRoughness = 0.0f;        // ����ֲڶ�
    };

}