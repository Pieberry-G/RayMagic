#include "RayTracing/RayTracingRenderer.h"

#include <random>
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

	RayTracingRenderer::RayTracingRenderer()
	{
        Sphere* s1 = new Sphere(glm::vec3(-0.65, -0.7, 0.0), 0.3, GREEN);
        Sphere* s2 = new Sphere(glm::vec3(0.0, -0.3, 0.0), 0.4, WHITE);
        Sphere* s3 = new Sphere(glm::vec3(0.65, 0.1, 0.0), 0.3, BLUE);
        s1->Material.SpecularRate = 0.3;
        s1->Material.Roughness = 0.1;

        s2->Material.SpecularRate = 0.3;
        s2->Material.RefractRate = 0.95;
        s2->Material.RefractAngle = 0.1;
        //s2.Material.refractRoughness = 0.05;

        s3->Material.SpecularRate = 0.3;

        shapes.push_back(s1);
        shapes.push_back(s2);
        shapes.push_back(s3);

        shapes.push_back(new Triangle(glm::vec3(-0.15, 0.4, -0.6), glm::vec3(-0.15, -0.95, -0.6), glm::vec3(0.15, 0.4, -0.6), YELLOW));
        shapes.push_back(new Triangle(glm::vec3(0.15, 0.4, -0.6), glm::vec3(-0.15, -0.95, -0.6), glm::vec3(0.15, -0.95, -0.6), YELLOW));

        Triangle tt = Triangle(glm::vec3(-0.2, -0.2, -0.95), glm::vec3(0.2, -0.2, -0.95), glm::vec3(-0.0, -0.9, 0.4), YELLOW);
        //tt.Material.specularRate = 0.1;
        //tt.Material.refractRate = 0.85;
        //tt.Material.refractRoughness = 0.3;
        //shapes.push_back(&tt);

        // ������
        Triangle* l1 = new Triangle(glm::vec3(0.4, 0.99, 0.4), glm::vec3(-0.4, 0.99, -0.4), glm::vec3(-0.4, 0.99, 0.4), WHITE);
        Triangle* l2 = new Triangle(glm::vec3(0.4, 0.99, 0.4), glm::vec3(0.4, 0.99, -0.4), glm::vec3(-0.4, 0.99, -0.4), WHITE);
        l1->Material.IsEmissive = true;
        l2->Material.IsEmissive = true;
        shapes.push_back(l1);
        shapes.push_back(l2);

        // ��������
        // bottom
        shapes.push_back(new Triangle(glm::vec3(1, -1, 1), glm::vec3(-1, -1, -1), glm::vec3(-1, -1, 1), WHITE));
        shapes.push_back(new Triangle(glm::vec3(1, -1, 1), glm::vec3(1, -1, -1), glm::vec3(-1, -1, -1), WHITE));
        // top
        shapes.push_back(new Triangle(glm::vec3(1, 1, 1), glm::vec3(-1, 1, 1), glm::vec3(-1, 1, -1), WHITE));
        shapes.push_back(new Triangle(glm::vec3(1, 1, 1), glm::vec3(-1, 1, -1), glm::vec3(1, 1, -1), WHITE));
        // back
        shapes.push_back(new Triangle(glm::vec3(1, -1, -1), glm::vec3(-1, 1, -1), glm::vec3(-1, -1, -1), CYAN));
        shapes.push_back(new Triangle(glm::vec3(1, -1, -1), glm::vec3(1, 1, -1), glm::vec3(-1, 1, -1), CYAN));
        // left
        shapes.push_back(new Triangle(glm::vec3(-1, -1, -1), glm::vec3(-1, 1, 1), glm::vec3(-1, -1, 1), BLUE));
        shapes.push_back(new Triangle(glm::vec3(-1, -1, -1), glm::vec3(-1, 1, -1), glm::vec3(-1, 1, 1), BLUE));
        // right
        shapes.push_back(new Triangle(glm::vec3(1, 1, 1), glm::vec3(1, -1, -1), glm::vec3(1, -1, 1), RED));
        shapes.push_back(new Triangle(glm::vec3(1, -1, -1), glm::vec3(1, 1, 1), glm::vec3(1, 1, -1), RED));
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
        res.Distance = std::numeric_limits<double>::max(); // inf

        // ��������ͼ�Σ����������
        for (auto& shape : shapes)
        {
            r = shape->Intersect(ray);
            if (r.IsHit && r.Distance < res.Distance) res = r;  // ��¼����������󽻽��
        }

        return res;
    }

    // 0-1 ���������
    std::uniform_real_distribution<> dis(0.0, 1.0);
    std::random_device rd;
    std::mt19937 gen(rd());
    double Randf()
    {
        return dis(gen);
    }

    // ��λ���ڵ��������
    glm::vec3 RandomVec3()
    {
        glm::vec3 d;
        do
        {
            d = 2.0f * glm::vec3(Randf(), Randf(), Randf()) - glm::vec3(1, 1, 1);
        } while (dot(d, d) > 1.0);
        return normalize(d);
    }

    // ��������������
    glm::vec3 RandomDirection(const glm::vec3& n)
    {
        return normalize(RandomVec3() + n);
    }

    // ·��׷��
    glm::vec3 PathTracing(std::vector<Shape*>& shapes, Ray ray, int depth)
    {
        if (depth > 8) return glm::vec3(0);
        HitResult res = Shoot(shapes, ray);

        if (!res.IsHit) return glm::vec3(0); // δ����

        // ��������򷵻���ɫ
        if (res.Material.IsEmissive) return res.Material.Color;

        // �� P �ĸ�����ֹ
        double r = Randf();
        float P = 0.8;
        if (r > P) return glm::vec3(0);

        // �������
        Ray randomRay;
        randomRay.StartPoint = res.HitPoint;
        randomRay.Direction = RandomDirection(res.Material.Normal);

        glm::vec3 color = glm::vec3(0);
        float cosine = fabs(dot(-ray.Direction, res.Material.Normal));

        // ���ݷ����ʾ����������յķ���
        r = Randf();
        if (r < res.Material.SpecularRate)  // ���淴��
        {
            glm::vec3 ref = normalize(reflect(ray.Direction, res.Material.Normal));
            randomRay.Direction = normalize(mix(ref, randomRay.Direction, res.Material.Roughness));
            color = PathTracing(shapes, randomRay, depth + 1) * cosine;
        }
        else if (res.Material.SpecularRate <= r && r <= res.Material.RefractRate)    // ����
        {
            glm::vec3 ref = glm::normalize(refract(ray.Direction, res.Material.Normal, float(res.Material.RefractAngle)));
            randomRay.Direction = normalize(mix(ref, -randomRay.Direction, res.Material.RefractRoughness));
            color = PathTracing(shapes, randomRay, depth + 1) * cosine;
        }
        else    // ������
        {
            glm::vec3 srcColor = res.Material.Color;
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
                ray.StartPoint = camera.GetPosition();
                ray.Direction = camera.GetRayDirections()[i * m_ImageWidth + j];

                // �볡���Ľ���
                HitResult res = Shoot(shapes, ray);
                glm::vec3 color = glm::vec3(0, 0, 0);

                if (res.IsHit)
                {
                    // ���й�Դֱ�ӷ��ع�Դ��ɫ
                    if (res.Material.IsEmissive)
                    {
                        color = res.Material.Color;
                    }
                    // ����ʵ����ѡ��һ������������·�����߲��ҽ���·��׷��
                    else
                    {
                        // ���ݽ��㴦���������ɽ��㴦����������������
                        Ray randomRay;
                        randomRay.StartPoint = res.HitPoint;
                        randomRay.Direction = RandomDirection(res.Material.Normal);

                        // ���ݷ����ʾ����������յķ���
                        double r = Randf();
                        if (r < res.Material.SpecularRate)  // ���淴��
                        {
                            glm::vec3 ref = normalize(reflect(ray.Direction, res.Material.Normal));
                            randomRay.Direction = normalize(mix(ref, randomRay.Direction, res.Material.Roughness));
                            color = PathTracing(shapes, randomRay, 0);
                        }
                        else if (res.Material.SpecularRate <= r && r <= res.Material.RefractRate)    // ����
                        {
                            glm::vec3 ref = normalize(refract(ray.Direction, res.Material.Normal, float(res.Material.RefractAngle)));
                            randomRay.Direction = normalize(mix(ref, -randomRay.Direction, res.Material.RefractRoughness));
                            color = PathTracing(shapes, randomRay, 0);
                        }
                        else    // ������
                        {
                            glm::vec3 srcColor = res.Material.Color;
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