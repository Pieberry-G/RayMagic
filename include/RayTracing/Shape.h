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

    // 三角形
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

        // 与光线求交
        virtual HitResult Intersect(Ray ray)
        {
            HitResult res;

            glm::vec3 S = ray.startPoint;       // 射线起点
            glm::vec3 d = ray.direction;        // 射线方向
            glm::vec3 N = material.normal;      // 法向量
            if (glm::dot(N, d) > 0.0f) N = -N;  // 获取正确的法向量

            // 如果视线和三角形平行
            if (fabs(dot(N, d)) < 0.00001f) return res;

            // 距离
            float t = (dot(N, p1) - dot(S, N)) / dot(d, N);
            if (t < 0.0005f) return res;        // 如果三角形在相机背面

            // 交点计算
            glm::vec3 P = S + d * t;

            // 判断交点是否在三角形中
            glm::vec3 c1 = cross(p2 - p1, P - p1);
            glm::vec3 c2 = cross(p3 - p2, P - p2);
            glm::vec3 c3 = cross(p1 - p3, P - p3);
            glm::vec3 n = material.normal;      // 需要 "原生法向量" 来判断
            if (dot(c1, n) < 0 || dot(c2, n) < 0 || dot(c3, n) < 0) return res;

            // 装填返回结果
            res.distance = t;
            res.hitPoint = P;
            res.material = material;
            res.material.normal = N;    // 要返回正确的法向
            return res;
        };

    public:
        glm::vec3 p1, p2, p3;       // 三顶点
        glm::vec3 center;
        Material material;   // 材质
    };

    // 球
    class Sphere : public Shape
    {
    public:
        Sphere(glm::vec3 O, float R, glm::vec3 C) { o = O; r = R; material.color = C; }

        // 与光线求交
        HitResult Intersect(Ray ray)
        {
            HitResult res;

            glm::vec3 S = ray.startPoint;        // 射线起点
            glm::vec3 d = ray.direction;         // 射线方向

            float OS = length(o - S);
            float SH = dot(o - S, d);
            float OH = sqrt(pow(OS, 2) - pow(SH, 2));

            if (OH > r) return res;     // OH大于半径则不相交

            float PH = sqrt(pow(r, 2) - pow(OH, 2));

            float t1 = glm::length(SH) - PH;
            float t2 = glm::length(SH) + PH;
            float t = (t1 < 0) ? (t2) : (t1);   // 最近距离
            glm::vec3 P = S + t * d;            // 交点

            // 防止自己交自己
            if (fabs(t1) < 0.0005f || fabs(t2) < 0.0005f) return res;

            // 装填返回结果
            res.distance = t;
            res.hitPoint = P;
            res.material = material;
            res.material.normal = normalize(P - o); // 要返回正确的法向
            return res;
        }

    public:
        glm::vec3 o;                // 圆心
        float r;                   // 半径
        Material material;   // 材质
    };

}