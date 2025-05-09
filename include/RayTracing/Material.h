#pragma once

#include <glm/glm.hpp>

namespace RayMagic {

    // ���������ʶ���
    struct Material
    {
        bool isEmissive = false;               // �Ƿ񷢹�
        glm::vec3 normal = glm::vec3(0, 0, 0); // ������
        glm::vec3 color = glm::vec3(0, 0, 0);  // ��ɫ
        double specularRate = 0.0f;            // �����ռ��
        double roughness = 1.0f;               // �ֲڳ̶�
        double refractRate = 0.0f;             // �����ռ��
        double refractAngle = 1.0f;            // ������
        double refractRoughness = 0.0f;        // ����ֲڶ�
    };

}