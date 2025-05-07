#pragma once

#include "RayTracing/Camera.h"
#include "RayTracing/Shape.h"

#include <glm/glm.hpp>

namespace RayMagic {

	class RayTracingRenderer
	{
	public:
		RayTracingRenderer();

		void Resize(uint32_t width, uint32_t height);
		void Render(const Camera& camera);
		uint32_t* GetImageData() { return m_ImageData; }

		void ResetFrameIndex() { m_FrameIndex = 1; }

	private:
		struct HitPayload
		{
			float HitDistance;
			glm::vec3 WorldPosition;
			glm::vec3 WorldNormal;

			uint32_t ObjectIndex;
		};

		//glm::vec4 PerPixel(uint32_t x, uint32_t y); //RayGen

		//HitPayload TraceRay(const Ray& ray);
		//HitPayload ClosestHit(const Ray& ray, float hitDistance, int objectIndex);
		//HitPayload Miss(const Ray& ray);

	private:
		uint32_t m_ImageWidth = 0, m_ImageHeight = 0;
		uint32_t* m_ImageData = nullptr;
		glm::vec4* m_AccumulationData = nullptr;

		uint32_t m_FrameIndex = 1;

		std::vector<Shape*> shapes;
	};

}