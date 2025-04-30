#pragma once

#include "Renderer/Texture.h"

#include <glm/glm.hpp>

namespace RayMagic {

	class RayTracingRenderer
	{
	public:
		void OnResize(uint32_t width, uint32_t height);
		//void Render(const Scene& scene, const Camera& camera);

		//std::shared_ptr<Walnut::Image> GetFinalImage() const { return m_FinalImage; }

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
		uint32_t* m_ImageData = nullptr;
	};

}