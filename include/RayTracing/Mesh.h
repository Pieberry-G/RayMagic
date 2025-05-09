#pragma once

#include "RayTracing/Shape.h"
#include "RayTracing/BVHTree.h"

#include <glm/glm.hpp>

namespace RayMagic {

    class Mesh : public Shape
    {
    public:
        Mesh(std::vector<Triangle>& triangles);
        Mesh(std::string filepath, glm::vec3 color);

        // 与光线求交
        HitResult Intersect(Ray ray)
        {
            return bvhTree->HitBVH(ray, bvhTree->GetRoot());
        };

    private:
        void ReadObj(std::string filepath, std::vector<glm::vec3>& vertices, std::vector<uint32_t>& indices);

    private:
        std::vector<Triangle> triangles;
        std::unique_ptr<BVHTree> bvhTree;
    };

}