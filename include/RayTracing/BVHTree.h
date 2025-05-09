#pragma once

#include "RayTracing/Shape.h"

#include <glm/glm.hpp>

namespace RayMagic {

	// ������������������ -- �ȽϺ���
	static bool cmpx(const Triangle& t1, const Triangle& t2) {
		return t1.center.x < t2.center.x;
	}
	static bool cmpy(const Triangle& t1, const Triangle& t2) {
		return t1.center.y < t2.center.y;
	}
	static bool cmpz(const Triangle& t1, const Triangle& t2) {
		return t1.center.z < t2.center.z;
	}

	// BVH ���ڵ�
	struct BVHNode
	{
		BVHNode* left = nullptr;    // ������������
		BVHNode* right = nullptr;
		int n, index;               // Ҷ�ӽڵ���Ϣ               
		glm::vec3 AA, BB;           // ��ײ��
	};

    class BVHTree
    {
	public:
		BVHTree(std::vector<Triangle>& Triangles);

		HitResult HitBVH(Ray ray, BVHNode* node);
		BVHNode* GetRoot() { return root; }

	private:
		BVHNode* BuildBVH(int l, int r, int n);
		BVHNode* BuildBVHwithSAH(int l, int r, int n);

		HitResult HitTriangleArray(Ray ray, int l, int r);
		float HitAABB(Ray r, glm::vec3 AA, glm::vec3 BB);

	private:
		std::vector<Triangle>& triangles;
		BVHNode* root = nullptr;
    };

}