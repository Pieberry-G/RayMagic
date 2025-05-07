#pragma once

#include "RayTracing/Ray.h"
#include "RayTracing/SurfaceMaterial.h"
#include <glm/glm.hpp>

namespace RayMagic {

    // 颜色
    const glm::vec3 RED(1, 0.5, 0.5);
    const glm::vec3 GREEN(0.5, 1, 0.5);
    const glm::vec3 BLUE(0.5, 0.5, 1);
    const glm::vec3 YELLOW(1.0, 1.0, 0.1);
    const glm::vec3 CYAN(0.1, 1.0, 1.0);
    const glm::vec3 MAGENTA(1.0, 0.1, 1.0);
    const glm::vec3 GRAY(0.5, 0.5, 0.5);
    const glm::vec3 WHITE(1, 1, 1);

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
        Triangle() = default;
        Triangle(glm::vec3 p1, glm::vec3 p2, glm::vec3 p3, glm::vec3 c)
        {
            P1 = p1, P2 = p2, P3 = p3;
            Material.Normal = normalize(cross(P2 - P1, P3 - P1));
            Material.Color = c;
        }

        // 与光线求交
        HitResult Intersect(Ray ray)
        {
            HitResult res;

            glm::vec3 S = ray.StartPoint;       // 射线起点
            glm::vec3 d = ray.Direction;        // 射线方向
            glm::vec3 N = Material.Normal;      // 法向量
            if (glm::dot(N, d) > 0.0f) N = -N;  // 获取正确的法向量

            // 如果视线和三角形平行
            if (fabs(dot(N, d)) < 0.00001f) return res;

            // 距离
            float t = (dot(N, P1) - dot(S, N)) / dot(d, N);
            if (t < 0.0005f) return res;        // 如果三角形在相机背面

            // 交点计算
            glm::vec3 P = S + d * t;

            // 判断交点是否在三角形中
            glm::vec3 c1 = cross(P2 - P1, P - P1);
            glm::vec3 c2 = cross(P3 - P2, P - P2);
            glm::vec3 c3 = cross(P1 - P3, P - P3);
            glm::vec3 n = Material.Normal;      // 需要 "原生法向量" 来判断
            if (dot(c1, n) < 0 || dot(c2, n) < 0 || dot(c3, n) < 0) return res;

            // 装填返回结果
            res.IsHit = true;
            res.Distance = t;
            res.HitPoint = P;
            res.Material = Material;
            res.Material.Normal = N;    // 要返回正确的法向
            return res;
        };

    public:
        glm::vec3 P1, P2, P3;       // 三顶点
        SurfaceMaterial Material;   // 材质
    };

    // 球
    class Sphere : public Shape
    {
    public:
        Sphere() = default;
        Sphere(glm::vec3 o, double r, glm::vec3 c) { O = o; R = r; Material.Color = c; }

        // 与光线求交
        HitResult Intersect(Ray ray)
        {
            HitResult res;

            glm::vec3 S = ray.StartPoint;        // 射线起点
            glm::vec3 d = ray.Direction;         // 射线方向

            float OS = length(O - S);
            float SH = dot(O - S, d);
            float OH = sqrt(pow(OS, 2) - pow(SH, 2));

            if (OH > R) return res;     // OH大于半径则不相交

            float PH = sqrt(pow(R, 2) - pow(OH, 2));

            float t1 = glm::length(SH) - PH;
            float t2 = glm::length(SH) + PH;
            float t = (t1 < 0) ? (t2) : (t1);   // 最近距离
            glm::vec3 P = S + t * d;            // 交点

            // 防止自己交自己
            if (fabs(t1) < 0.0005f || fabs(t2) < 0.0005f) return res;

            // 装填返回结果
            res.IsHit = true;
            res.Distance = t;
            res.HitPoint = P;
            res.Material = Material;
            res.Material.Normal = normalize(P - O); // 要返回正确的法向
            return res;
        }

    public:
        glm::vec3 O;                // 圆心
        double R;                   // 半径
        SurfaceMaterial Material;   // 材质
    };

}