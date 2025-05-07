#pragma once

#include "RayMagic.h"

#include "RayTracing/RayTracingRenderer.h"
#include "RayTracing/Camera.h"

namespace RayMagic {

	class EditorLayer : public Layer
	{
	public:
		EditorLayer();
		virtual ~EditorLayer() = default;

		virtual void OnAttach() override;
		virtual void OnDetach() override;

		virtual void OnUpdate(float ts) override;
		virtual void OnImGuiRender() override;
		virtual void OnEvent(Event& e) override;

	private:
		std::shared_ptr<FrameBuffer> m_FrameBuffer;

		RayTracingRenderer m_RayTracingRenderer;
		Camera m_RayTracingCamera;

		bool m_ViewportFocused = false, m_ViewportHovered = false;
		glm::vec2 m_ViewportSize = { 0.0f, 0.0f };

		float m_LastRenderTime = 0.0f;
	};

}