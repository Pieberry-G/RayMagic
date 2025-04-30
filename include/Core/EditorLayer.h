#pragma once

#include "RayMagic.h"

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
		OrthographicCameraController m_CameraController;

		std::shared_ptr<FrameBuffer> m_FrameBuffer;

		std::shared_ptr<Texture2D> m_CheckerboardTexture;
		std::shared_ptr<Texture2D> m_SpriteSheet;

		struct ProfileResult
		{
			const char* Name;
			float Time;
		};

		std::vector<ProfileResult> m_ProfileResults;

		bool m_ViewportFocused = false, m_ViewportHovered = false;
		glm::vec2 m_ViewportSize = { 0.0f, 0.0f };

		glm::vec4 m_SquareColor = { 0.2f, 0.3f, 0.8f, 1.0f };
	};

}