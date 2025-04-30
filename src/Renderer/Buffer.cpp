#include "Renderer/Buffer.h"

#include "Renderer/Renderer.h"
#include "Platform/OpenGL/OpenGLBuffer.h"

namespace RayMagic {

	std::shared_ptr<VertexBuffer> VertexBuffer::Create(uint32_t size)
	{
		switch (Renderer::GetAPI())
		{
			case RendererAPI::API::None:    RM_CORE_ASSERT(false, "RendererAPI::None is currently not supported!"); return nullptr;
			case RendererAPI::API::OpenGL:  return std::make_shared<OpenGLVertexBuffer>(size);
		}

		RM_CORE_ASSERT(false, "Unknown RendererAPI!");
		return nullptr;
	}

	std::shared_ptr<VertexBuffer> RayMagic::VertexBuffer::Create(float* vertices, uint32_t size)
	{
		switch (Renderer::GetAPI())
		{
			case RendererAPI::API::None:    RM_CORE_ASSERT(false, "RendererAPI::None is currently not supported!"); return nullptr;
			case RendererAPI::API::OpenGL:  return std::make_shared<OpenGLVertexBuffer>(vertices, size);
		}

		RM_CORE_ASSERT(false, "Unknown RendererAPI!");
		return nullptr;
	}

	std::shared_ptr<IndexBuffer> RayMagic::IndexBuffer::Create(uint32_t* indices, uint32_t count)
	{
		switch (Renderer::GetAPI())
		{
			case RendererAPI::API::None:    RM_CORE_ASSERT(false, "RendererAPI::None is currently not supported!"); return nullptr;
			case RendererAPI::API::OpenGL:  return std::make_shared<OpenGLIndexBuffer>(indices, count);
		}

		RM_CORE_ASSERT(false, "Unknown RendererAPI!");
		return nullptr;
	}
}
