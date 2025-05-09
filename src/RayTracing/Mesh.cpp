#include "RayTracing/Mesh.h"

namespace RayMagic {

    Mesh::Mesh(std::vector<Triangle>& triangles)
        : triangles(triangles)
    {
        bvhTree = std::make_unique<BVHTree>(triangles);
    }

    Mesh::Mesh(std::string filepath, glm::vec3 color)
    {
        std::vector<glm::vec3> vertices;     // 顶点坐标
        std::vector<uint32_t> indices;    // 顶点索引
        ReadObj(filepath, vertices, indices);
        for (auto& v : vertices) {
            v.x *= 5.0, v.y *= 5.0, v.z *= 5.0;
            v.y -= 0.5;
        }
        for (int i = 0; i < indices.size(); i += 3) {
            triangles.push_back(Triangle(vertices[indices[i]], vertices[indices[i + 1]], vertices[indices[i + 2]], color));
        }

        bvhTree = std::make_unique<BVHTree>(triangles);
    }

    void Mesh::ReadObj(std::string filepath, std::vector<glm::vec3>& vertices, std::vector<uint32_t>& indices)
    {
        // 打开文件流
        std::ifstream fin(filepath);
        std::string line;
        if (!fin.is_open()) {
            std::cout << "File: " << filepath << " Open Error!" << std::endl;
            //exit(-1);
        }

        // 增量读取
        int offset = vertices.size();

        // 按行读取
        while (std::getline(fin, line)) {
            std::istringstream sin(line);   // 以一行的数据作为 string stream 解析并且读取
            std::string type;
            float x, y, z;
            int v0, v1, v2;

            // 读取obj文件
            sin >> type;
            if (type == "v") {
                sin >> x >> y >> z;
                vertices.push_back(glm::vec3(x, y, z));
            }
            if (type == "f") {
                sin >> v0 >> v1 >> v2;
                indices.push_back(v0 - 1 + offset);
                indices.push_back(v1 - 1 + offset);
                indices.push_back(v2 - 1 + offset);
            }
        }
    }

}