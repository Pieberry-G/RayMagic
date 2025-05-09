#pragma once

#include "RayTracing/Ray.h"
#include "RayTracing/Material.h"

#include <glm/glm.hpp>

namespace RayMagic {

	class Shape
	{
	public:
		Shape() = default;
        virtual HitResult Intersect(Ray ray) = 0;
	};

    // ������
    class Triangle : public Shape
    {
    public:
        Triangle(glm::vec3 P1, glm::vec3 P2, glm::vec3 P3, glm::vec3 C)
        {
            p1 = P1, p2 = P2, p3 = P3;
            center = (p1 + p2 + p3) / 3.0f;
            material.normal = normalize(cross(p2 - p1, p3 - p1));
            material.color = C;
        }

        // �������
        virtual HitResult Intersect(Ray ray)
        {
            HitResult res;

            glm::vec3 S = ray.startPoint;       // �������
            glm::vec3 d = ray.direction;        // ���߷���
            glm::vec3 N = material.normal;      // ������
            if (glm::dot(N, d) > 0.0f) N = -N;  // ��ȡ��ȷ�ķ�����

            // ������ߺ�������ƽ��
            if (fabs(dot(N, d)) < 0.00001f) return res;

            // ����
            float t = (dot(N, p1) - dot(S, N)) / dot(d, N);
            if (t < 0.0005f) return res;        // ������������������

            // �������
            glm::vec3 P = S + d * t;

            // �жϽ����Ƿ�����������
            glm::vec3 c1 = cross(p2 - p1, P - p1);
            glm::vec3 c2 = cross(p3 - p2, P - p2);
            glm::vec3 c3 = cross(p1 - p3, P - p3);
            glm::vec3 n = material.normal;      // ��Ҫ "ԭ��������" ���ж�
            if (dot(c1, n) < 0 || dot(c2, n) < 0 || dot(c3, n) < 0) return res;

            // װ��ؽ��
            res.distance = t;
            res.hitPoint = P;
            res.material = material;
            res.material.normal = N;    // Ҫ������ȷ�ķ���
            return res;
        };

    public:
        glm::vec3 p1, p2, p3;       // ������
        glm::vec3 center;
        Material material;   // ����
    };

    // ��
    class Sphere : public Shape
    {
    public:
        Sphere(glm::vec3 O, float R, glm::vec3 C) { o = O; r = R; material.color = C; }

        // �������
        HitResult Intersect(Ray ray)
        {
            HitResult res;

            glm::vec3 S = ray.startPoint;        // �������
            glm::vec3 d = ray.direction;         // ���߷���

            float OS = length(o - S);
            float SH = dot(o - S, d);
            float OH = sqrt(pow(OS, 2) - pow(SH, 2));

            if (OH > r) return res;     // OH���ڰ뾶���ཻ

            float PH = sqrt(pow(r, 2) - pow(OH, 2));

            float t1 = glm::length(SH) - PH;
            float t2 = glm::length(SH) + PH;
            float t = (t1 < 0) ? (t2) : (t1);   // �������
            glm::vec3 P = S + t * d;            // ����

            // ��ֹ�Լ����Լ�
            if (fabs(t1) < 0.0005f || fabs(t2) < 0.0005f) return res;

            // װ��ؽ��
            res.distance = t;
            res.hitPoint = P;
            res.material = material;
            res.material.normal = normalize(P - o); // Ҫ������ȷ�ķ���
            return res;
        }

    public:
        glm::vec3 o;                // Բ��
        float r;                   // �뾶
        Material material;   // ����
    };

}