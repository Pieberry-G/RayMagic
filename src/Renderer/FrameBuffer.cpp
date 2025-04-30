#include "Renderer/FrameBuffer.h"

#include "Renderer/Renderer.h"
#include "Platform/OpenGL/OpenGLFrameBuffer.h"

namespace RayMagic {

	std::shared_ptr<FrameBuffer> FrameBuffer::Create(const FramebufferSpecification& spec)
	{
		switch (Renderer::GetAPI())
		{
			case RendererAPI::API::None:    RM_CORE_ASSERT(false, "RendererAPI::None is currently not supported!"); return nullptr;
			case RendererAPI::API::OpenGL:  return std::make_shared<OpenGLFrameBuffer>(spec);
		}

		RM_CORE_ASSERT(false, "Unknown RendererAPI!");
		return nullptr;
	}

}