#include "Renderer/RenderCommand.h"

#include "Platform/OpenGL/OpenGLRendererAPI.h"

namespace RayMagic {

	RendererAPI* RenderCommand::s_RendererAPI = new OpenGLRendererAPI();

}