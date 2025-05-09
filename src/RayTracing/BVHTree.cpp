#include "RayTracing/BVHTree.h"

namespace RayMagic {

    BVHTree::BVHTree(std::vector<Triangle>& Triangles)
        : triangles(Triangles)
    {
        root = BuildBVHwithSAH(0, triangles.size() - 1, 8);
    }

    // ����������
    HitResult BVHTree::HitTriangleArray(Ray ray, int l, int r)
    {
        HitResult res;
        for (int i = l; i <= r; i++)
        {
            HitResult r = triangles[i].Intersect(ray);
            if (r.distance < res.distance) res = r;
        }
        return res;
    }

    // �� aabb �����󽻣�û�н����򷵻� -1
    float BVHTree::HitAABB(Ray r, glm::vec3 AA, glm::vec3 BB)
    {
        // 1.0 / direction
        glm::vec3 invdir = glm::vec3(1.0 / r.direction.x, 1.0 / r.direction.y, 1.0 / r.direction.z);

        glm::vec3 in = (BB - r.startPoint) * invdir;
        glm::vec3 out = (AA - r.startPoint) * invdir;

        glm::vec3 tmax = max(in, out);
        glm::vec3 tmin = min(in, out);

        float t1 = std::min(tmax.x, std::min(tmax.y, tmax.z));
        float t0 = std::max(tmin.x, std::max(tmin.y, tmin.z));

        return (t1 >= t0) ? ((t0 > 0.0) ? (t0) : (t1)) : (-1);
    }

    // �� BVH �ϱ�����
    HitResult BVHTree::HitBVH(Ray ray, BVHNode* node)
    {
        if (node == nullptr) return HitResult();

        // ��Ҷ�� ������
        if (node->n > 0)
            return HitTriangleArray(ray, node->n, node->n + node->index - 1);

        // ���������� AABB ��
        float d1 = FLT_MAX, d2 = FLT_MAX;
        if (node->left) d1 = HitAABB(ray, node->left->AA, node->left->BB);
        if (node->right) d2 = HitAABB(ray, node->right->AA, node->right->BB);

        // �ݹ���
        HitResult r1, r2;
        if (d1 > 0) r1 = HitBVH(ray, node->left);
        if (d2 > 0) r2 = HitBVH(ray, node->right);

        return r1.distance < r2.distance ? r1 : r2;
    }

    // ���� BVH
    BVHNode* BVHTree::BuildBVH(int l, int r, int n)
    {
        if (l > r) return 0;

        BVHNode* node = new BVHNode();
        node->AA = glm::vec3(FLT_MAX, FLT_MAX, FLT_MAX);
        node->BB = glm::vec3(FLT_MIN, FLT_MIN, FLT_MIN);

        // ���� AABB
        for (int i = l; i <= r; i++)
        {
            // ��С�� AA
            float minx = std::min(triangles[i].p1.x, std::min(triangles[i].p2.x, triangles[i].p3.x));
            float miny = std::min(triangles[i].p1.y, std::min(triangles[i].p2.y, triangles[i].p3.y));
            float minz = std::min(triangles[i].p1.z, std::min(triangles[i].p2.z, triangles[i].p3.z));
            node->AA.x = std::min(node->AA.x, minx);
            node->AA.y = std::min(node->AA.y, miny);
            node->AA.z = std::min(node->AA.z, minz);
            // ���� BB
            float maxx = std::max(triangles[i].p1.x, std::max(triangles[i].p2.x, triangles[i].p3.x));
            float maxy = std::max(triangles[i].p1.y, std::max(triangles[i].p2.y, triangles[i].p3.y));
            float maxz = std::max(triangles[i].p1.z, std::max(triangles[i].p2.z, triangles[i].p3.z));
            node->BB.x = std::max(node->BB.x, maxx);
            node->BB.y = std::max(node->BB.y, maxy);
            node->BB.z = std::max(node->BB.z, maxz);
        }

        // ������ n �������� ����Ҷ�ӽڵ�
        if ((r - l + 1) <= n)
        {
            node->n = r - l + 1;
            node->index = l;
            return node;
        }

        // ����ݹ齨��
        float lenx = node->BB.x - node->AA.x;
        float leny = node->BB.y - node->AA.y;
        float lenz = node->BB.z - node->AA.z;
        // �� x ����
        if (lenx >= leny && lenx >= lenz)
            std::sort(triangles.begin() + l, triangles.begin() + r + 1, cmpx);
        // �� y ����
        if (leny >= lenx && leny >= lenz)
            std::sort(triangles.begin() + l, triangles.begin() + r + 1, cmpy);
        // �� z ����
        if (lenz >= lenx && lenz >= leny)
            std::sort(triangles.begin() + l, triangles.begin() + r + 1, cmpz);

        // �ݹ�
        int mid = (l + r) / 2;
        node->left = BuildBVH(l, mid, n);
        node->right = BuildBVH(mid + 1, r, n);

        return node;
    }

    // SAH �Ż����� BVH
    BVHNode* BVHTree::BuildBVHwithSAH(int l, int r, int n)
    {
        if (l > r) return 0;

        BVHNode* node = new BVHNode();
        node->AA = glm::vec3(FLT_MAX, FLT_MAX, FLT_MAX);
        node->BB = glm::vec3(FLT_MIN, FLT_MIN, FLT_MIN);

        // ���� AABB
        for (int i = l; i <= r; i++) {
            // ��С�� AA
            float minx = std::min(triangles[i].p1.x, std::min(triangles[i].p2.x, triangles[i].p3.x));
            float miny = std::min(triangles[i].p1.y, std::min(triangles[i].p2.y, triangles[i].p3.y));
            float minz = std::min(triangles[i].p1.z, std::min(triangles[i].p2.z, triangles[i].p3.z));
            node->AA.x = std::min(node->AA.x, minx);
            node->AA.y = std::min(node->AA.y, miny);
            node->AA.z = std::min(node->AA.z, minz);
            // ���� BB
            float maxx = std::max(triangles[i].p1.x, std::max(triangles[i].p2.x, triangles[i].p3.x));
            float maxy = std::max(triangles[i].p1.y, std::max(triangles[i].p2.y, triangles[i].p3.y));
            float maxz = std::max(triangles[i].p1.z, std::max(triangles[i].p2.z, triangles[i].p3.z));
            node->BB.x = std::max(node->BB.x, maxx);
            node->BB.y = std::max(node->BB.y, maxy);
            node->BB.z = std::max(node->BB.z, maxz);
        }

        // ������ n �������� ����Ҷ�ӽڵ�
        if ((r - l + 1) <= n) {
            node->n = r - l + 1;
            node->index = l;
            return node;
        }

        // ����ݹ齨��
        float Cost = FLT_MAX;
        int Axis = 0;
        int Split = (l + r) / 2;
        for (int axis = 0; axis < 3; axis++) {
            // �ֱ� x��y��z ������
            if (axis == 0) std::sort(&triangles[0] + l, &triangles[0] + r + 1, cmpx);
            if (axis == 1) std::sort(&triangles[0] + l, &triangles[0] + r + 1, cmpy);
            if (axis == 2) std::sort(&triangles[0] + l, &triangles[0] + r + 1, cmpz);

            // leftstd::max[i]: [l, i] ������ xyz ֵ
            // leftstd::min[i]: [l, i] ����С�� xyz ֵ
            std::vector<glm::vec3> leftmax(r - l + 1, glm::vec3(FLT_MIN, FLT_MIN, FLT_MIN));
            std::vector<glm::vec3> leftmin(r - l + 1, glm::vec3(FLT_MAX, FLT_MAX, FLT_MAX));
            // ����ǰ׺ ע�� i-l �Զ��뵽�±� 0
            for (int i = l; i <= r; i++) {
                Triangle& t = triangles[i];
                int bias = (i == l) ? 0 : 1;  // ��һ��Ԫ�����⴦��

                leftmax[i - l].x = std::max(leftmax[i - l - bias].x, std::max(t.p1.x, std::max(t.p2.x, t.p3.x)));
                leftmax[i - l].y = std::max(leftmax[i - l - bias].y, std::max(t.p1.y, std::max(t.p2.y, t.p3.y)));
                leftmax[i - l].z = std::max(leftmax[i - l - bias].z, std::max(t.p1.z, std::max(t.p2.z, t.p3.z)));

                leftmin[i - l].x = std::min(leftmin[i - l - bias].x, std::min(t.p1.x, std::min(t.p2.x, t.p3.x)));
                leftmin[i - l].y = std::min(leftmin[i - l - bias].y, std::min(t.p1.y, std::min(t.p2.y, t.p3.y)));
                leftmin[i - l].z = std::min(leftmin[i - l - bias].z, std::min(t.p1.z, std::min(t.p2.z, t.p3.z)));
            }

            // rightstd::max[i]: [i, r] ������ xyz ֵ
            // rightstd::min[i]: [i, r] ����С�� xyz ֵ
            std::vector<glm::vec3> rightmax(r - l + 1, glm::vec3(FLT_MIN, FLT_MIN, FLT_MIN));
            std::vector<glm::vec3> rightmin(r - l + 1, glm::vec3(FLT_MAX, FLT_MAX, FLT_MAX));
            // �����׺ ע�� i-l �Զ��뵽�±� 0
            for (int i = r; i >= l; i--) {
                Triangle& t = triangles[i];
                int bias = (i == r) ? 0 : 1;  // ��һ��Ԫ�����⴦��

                rightmax[i - l].x = std::max(rightmax[i - l + bias].x, std::max(t.p1.x, std::max(t.p2.x, t.p3.x)));
                rightmax[i - l].y = std::max(rightmax[i - l + bias].y, std::max(t.p1.y, std::max(t.p2.y, t.p3.y)));
                rightmax[i - l].z = std::max(rightmax[i - l + bias].z, std::max(t.p1.z, std::max(t.p2.z, t.p3.z)));

                rightmin[i - l].x = std::min(rightmin[i - l + bias].x, std::min(t.p1.x, std::min(t.p2.x, t.p3.x)));
                rightmin[i - l].y = std::min(rightmin[i - l + bias].y, std::min(t.p1.y, std::min(t.p2.y, t.p3.y)));
                rightmin[i - l].z = std::min(rightmin[i - l + bias].z, std::min(t.p1.z, std::min(t.p2.z, t.p3.z)));
            }

            // ����Ѱ�ҷָ�
            float cost = FLT_MAX;
            int split = l;
            for (int i = l; i <= r - 1; i++) {
                float lenx, leny, lenz;
                // ��� [l, i]
                glm::vec3 leftAA = leftmin[i - l];
                glm::vec3 leftBB = leftmax[i - l];
                lenx = leftBB.x - leftAA.x;
                leny = leftBB.y - leftAA.y;
                lenz = leftBB.z - leftAA.z;
                float leftS = 2.0 * ((lenx * leny) + (lenx * lenz) + (leny * lenz));
                float leftCost = leftS * (i - l + 1);

                // �Ҳ� [i+1, r]
                glm::vec3 rightAA = rightmin[i + 1 - l];
                glm::vec3 rightBB = rightmax[i + 1 - l];
                lenx = rightBB.x - rightAA.x;
                leny = rightBB.y - rightAA.y;
                lenz = rightBB.z - rightAA.z;
                float rightS = 2.0 * ((lenx * leny) + (lenx * lenz) + (leny * lenz));
                float rightCost = rightS * (r - i);

                // ��¼ÿ���ָ����С��
                float totalCost = leftCost + rightCost;
                if (totalCost < cost) {
                    cost = totalCost;
                    split = i;
                }
            }
            // ��¼ÿ�������Ѵ�
            if (cost < Cost) {
                Cost = cost;
                Axis = axis;
                Split = split;
            }
        }

        // �������ָ�
        if (Axis == 0) std::sort(&triangles[0] + l, &triangles[0] + r + 1, cmpx);
        if (Axis == 1) std::sort(&triangles[0] + l, &triangles[0] + r + 1, cmpy);
        if (Axis == 2) std::sort(&triangles[0] + l, &triangles[0] + r + 1, cmpz);

        // �ݹ�
        node->left = BuildBVHwithSAH(l, Split, n);
        node->right = BuildBVHwithSAH(Split + 1, r, n);

        return node;
    }

}