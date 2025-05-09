#include "RayTracing/RayTracingRenderer.h"

#include "RayTracing/Mesh.h"
#include "RayTracing/Random.h"

#include <execution>

namespace RayMagic {

	namespace Utils {

		static uint32_t ConvertToRGBA(glm::vec4& color)
		{
			uint8_t r = (uint8_t)(color.r * 255.0f);
			uint8_t g = (uint8_t)(color.g * 255.0f);
			uint8_t b = (uint8_t)(color.b * 255.0f);
			uint8_t a = (uint8_t)(color.a * 255.0f);

			uint32_t result = a << 24 | b << 16 | g << 8 | r;
			return result;
		}
	}

    // ��ɫ
    const glm::vec3 RED(1.0, 0.0, 0.0);
    const glm::vec3 GREEN(0.0, 1.0, 0.0);
    const glm::vec3 BLUE(0.0, 0.0, 1.0);
    const glm::vec3 YELLOW(1.0, 1.0, 0.0);
    const glm::vec3 CYAN(0.1, 1.0, 1.0);
    const glm::vec3 MAGENTA(1.0, 0.1, 1.0);
    const glm::vec3 GRAY(0.5, 0.5, 0.5);
    const glm::vec3 WHITE(1, 1, 1);
    const glm::vec3 VIOLET(0.545, 0.0, 1.0);


	RayTracingRenderer::RayTracingRenderer()
	{
        Sphere* s1 = new Sphere(glm::vec3(0.65, -0.7, 0.0), 0.3, CYAN);
        Sphere* s2 = new Sphere(glm::vec3(0.0, -0.3, 0.0), 0.4, WHITE);
        Sphere* s3 = new Sphere(glm::vec3(-0.65, 0.1, 0.0), 0.3, VIOLET);
        s1->material.specularRate = 0.3;
        s1->material.roughness = 0.1;

        s2->material.specularRate = 0.3;
        s2->material.refractRate = 0.95;
        s2->material.refractAngle = 0.1;
        //s2.Material.refractRoughness = 0.05;

        s3->material.specularRate = 0.3;

        shapes.push_back(s1);
        shapes.push_back(s2);
        shapes.push_back(s3);

        shapes.push_back(new Triangle(glm::vec3(-0.15, 0.4, -0.6), glm::vec3(-0.15, -0.95, -0.6), glm::vec3(0.15, 0.4, -0.6), YELLOW));
        shapes.push_back(new Triangle(glm::vec3(0.15, 0.4, -0.6), glm::vec3(-0.15, -0.95, -0.6), glm::vec3(0.15, -0.95, -0.6), YELLOW));

        // ��������
        // bottom
        shapes.push_back(new Triangle(glm::vec3(1, -1, 1), glm::vec3(-1, -1, -1), glm::vec3(-1, -1, 1), WHITE));
        shapes.push_back(new Triangle(glm::vec3(1, -1, 1), glm::vec3(1, -1, -1), glm::vec3(-1, -1, -1), WHITE));
        // top
        shapes.push_back(new Triangle(glm::vec3(1, 1, 1), glm::vec3(-1, 1, 1), glm::vec3(-1, 1, -1), WHITE));
        shapes.push_back(new Triangle(glm::vec3(1, 1, 1), glm::vec3(-1, 1, -1), glm::vec3(1, 1, -1), WHITE));
        // back
        shapes.push_back(new Triangle(glm::vec3(1, -1, -1), glm::vec3(-1, 1, -1), glm::vec3(-1, -1, -1), WHITE));
        shapes.push_back(new Triangle(glm::vec3(1, -1, -1), glm::vec3(1, 1, -1), glm::vec3(-1, 1, -1), WHITE));
        // left
        shapes.push_back(new Triangle(glm::vec3(-1, -1, -1), glm::vec3(-1, 1, 1), glm::vec3(-1, -1, 1), RED));
        shapes.push_back(new Triangle(glm::vec3(-1, -1, -1), glm::vec3(-1, 1, -1), glm::vec3(-1, 1, 1), RED));
        // right
        shapes.push_back(new Triangle(glm::vec3(1, 1, 1), glm::vec3(1, -1, -1), glm::vec3(1, -1, 1), GREEN));
        shapes.push_back(new Triangle(glm::vec3(1, -1, -1), glm::vec3(1, 1, 1), glm::vec3(1, 1, -1), GREEN));

        //shapes.clear();

        // ������
        Triangle* l1 = new Triangle(glm::vec3(0.4, 0.99, 0.4), glm::vec3(-0.4, 0.99, -0.4), glm::vec3(-0.4, 0.99, 0.4), WHITE);
        Triangle* l2 = new Triangle(glm::vec3(0.4, 0.99, 0.4), glm::vec3(0.4, 0.99, -0.4), glm::vec3(-0.4, 0.99, -0.4), WHITE);
        l1->material.isEmissive = true;
        l2->material.isEmissive = true;
        shapes.push_back(l1);
        shapes.push_back(l2);

        //shapes.push_back(new Mesh("../assets/meshes/bunny.obj", RED));
	}

	void RayTracingRenderer::Resize(uint32_t width, uint32_t height)
	{
		m_ImageWidth = width;
		m_ImageHeight = height;
		delete[] m_ImageData;
		m_ImageData = new uint32_t[width * height];
        delete[] m_AccumulationData;
        m_AccumulationData = new glm::vec4[width * height];
	}

    // ���ؾ������ hit �Ľ��
    HitResult Shoot(std::vector<Shape*>& shapes, Ray ray)
    {
        HitResult res, r;

        // ��������ͼ�Σ����������
        for (auto& shape : shapes)
        {
            r = shape->Intersect(ray);
            if (r.distance < res.distance) res = r;  // ��¼����������󽻽��
        }

        return res;
    }

    // ·��׷��
    glm::vec3 PathTracing(std::vector<Shape*>& shapes, Ray ray, int depth)
    {
        if (depth > 8) return glm::vec3(0);
        HitResult res = Shoot(shapes, ray);

        if (res.distance == FLT_MAX) return glm::vec3(0); // δ����

        // ��������򷵻���ɫ
        if (res.material.isEmissive) return res.material.color;

        // �� P �ĸ�����ֹ
        double r = Random::Float();
        float P = 0.8;
        if (r > P) return glm::vec3(0);

        // �������
        Ray randomRay;
        randomRay.startPoint = res.hitPoint;
        randomRay.direction = Random::InNormalHemisphere(res.material.normal);

        glm::vec3 color = glm::vec3(0);
        float cosine = fabs(dot(-ray.direction, res.material.normal));

        // ���ݷ����ʾ����������յķ���
        r = Random::Float();
        if (r < res.material.specularRate)  // ���淴��
        {
            glm::vec3 ref = normalize(reflect(ray.direction, res.material.normal));
            randomRay.direction = normalize(mix(ref, randomRay.direction, res.material.roughness));
            color = PathTracing(shapes, randomRay, depth + 1) * cosine;
        }
        else if (res.material.specularRate <= r && r <= res.material.refractRate)    // ����
        {
            glm::vec3 ref = glm::normalize(refract(ray.direction, res.material.normal, float(res.material.refractAngle)));
            randomRay.direction = normalize(mix(ref, -randomRay.direction, res.material.refractRoughness));
            color = PathTracing(shapes, randomRay, depth + 1) * cosine;
        }
        else    // ������
        {
            glm::vec3 srcColor = res.material.color;
            glm::vec3 ptColor = PathTracing(shapes, randomRay, depth + 1) * cosine;
            color = ptColor * srcColor;    // ��ԭ��ɫ���
        }

        return color / P;
    }

    void RayTracingRenderer::Render(const Camera& camera)
    {
        if (m_FrameIndex == 1)
        {
            memset(m_AccumulationData, 0, m_ImageWidth * m_ImageHeight * sizeof(glm::vec4));
            memset(m_ImageData, 0, m_ImageWidth * m_ImageHeight * sizeof(uint32_t));
        }

        const double BRIGHTNESS = (2.0f * 3.1415926f);
        const double SCREEN_Z = 1.1;        // ��ƽ�� z ����
        const glm::vec3 EYE = glm::vec3(0, 0, 4.0);   // ���λ��
        std::vector<uint32_t> m_ImageVerticalIter(m_ImageHeight);
        for (uint32_t i = 0; i < m_ImageHeight; i++) {
            m_ImageVerticalIter[i] = i;
        }
        std::for_each(std::execution::par, m_ImageVerticalIter.begin(), m_ImageVerticalIter.end(), [=](uint32_t i)
        {
            for (uint32_t j = 0; j < m_ImageWidth; j++)
            {
                Ray ray;
                ray.startPoint = camera.GetPosition();
                ray.direction = camera.GetRayDirections()[i * m_ImageWidth + j];

                // �볡���Ľ���
                HitResult res = Shoot(shapes, ray);
                glm::vec3 color = glm::vec3(0, 0, 0);

                if (res.IsHit())
                {
                    // ���й�Դֱ�ӷ��ع�Դ��ɫ
                    if (res.material.isEmissive)
                    {
                        color = res.material.color;
                    }
                    // ����ʵ����ѡ��һ������������·�����߲��ҽ���·��׷��
                    else
                    {
                        // ���ݽ��㴦���������ɽ��㴦����������������
                        Ray randomRay;
                        randomRay.startPoint = res.hitPoint;
                        randomRay.direction = Random::InNormalHemisphere(res.material.normal);

                        // ���ݷ����ʾ����������յķ���
                        double r = Random::Float();
                        if (r < res.material.specularRate)  // ���淴��
                        {
                            glm::vec3 ref = normalize(reflect(ray.direction, res.material.normal));
                            randomRay.direction = normalize(mix(ref, randomRay.direction, res.material.roughness));
                            color = PathTracing(shapes, randomRay, 0);
                        }
                        else if (res.material.specularRate <= r && r <= res.material.refractRate)    // ����
                        {
                            glm::vec3 ref = normalize(refract(ray.direction, res.material.normal, float(res.material.refractAngle)));
                            randomRay.direction = normalize(mix(ref, -randomRay.direction, res.material.refractRoughness));
                            color = PathTracing(shapes, randomRay, 0);
                        }
                        else    // ������
                        {
                            glm::vec3 srcColor = res.material.color;
                            glm::vec3 ptColor = PathTracing(shapes, randomRay, 0);
                            color = ptColor * srcColor;    // ��ԭ��ɫ���
                        }
                        color *= BRIGHTNESS;
                    }

                    m_AccumulationData[i * m_ImageWidth + j] += glm::vec4(color, 1.0f);

                    glm::vec4 accumualtionColor = m_AccumulationData[i * m_ImageWidth + j];
                    accumualtionColor /= (float)m_FrameIndex;
                    accumualtionColor = glm::clamp(accumualtionColor, glm::vec4(0.0f), glm::vec4(1.0f));

                    // ٤�����
                    glm::vec4 gammaColor = glm::pow(accumualtionColor, glm::vec4(1.0f / 2.2));
                    gammaColor.w = accumualtionColor.w;
                    m_ImageData[i * m_ImageWidth + j] = Utils::ConvertToRGBA(gammaColor);
                }
            }
        });
        m_FrameIndex++;
	}

}