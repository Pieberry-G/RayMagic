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
		uint32_t m_ImageWidth = 0, m_ImageHeight = 0;
		uint32_t* m_ImageData = nullptr;
		glm::vec4* m_AccumulationData = nullptr;

		uint32_t m_FrameIndex = 1;

		std::vector<Shape*> shapes;
	};

}