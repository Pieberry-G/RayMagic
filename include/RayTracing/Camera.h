#pragma once

#include "Camera.h"
#include "Core/Timestep.h"
#include "Events/Event.h"
#include "Events/MouseEvent.h"

#include <glm/glm.hpp>

namespace RayMagic {

	class Camera
	{ 
	public:
		Camera() = default;
		Camera(float fov, float aspectRatio, float nearClip, float farClip);

		bool OnUpdate(Timestep ts);
		void OnEvent(Event& e);

		float GetDistance() const { return m_Distance; }
		void SetDistance(float distance) { m_Distance = distance; }

		void SetViewportSize(float width, float height) { m_ViewportWidth = width; m_ViewportHeight = height; UpdateProjection(); UpdateRayDirections(); }

		const glm::mat4& GetProjection() const { return m_Projection; }
		const glm::mat4& GetInverseProjection() const { return m_InverseProjection; }
		const glm::mat4& GetView() const { return m_View; }
		const glm::mat4& GetInverseView() const { return m_InverseView; }
		glm::mat4 GetViewProjection() const { return m_Projection * m_View; }
		const std::vector<glm::vec3>& GetRayDirections() const { return m_RayDirections; }

		glm::vec3 GetUpDirection() const;
		glm::vec3 GetRightDirection() const;
		glm::vec3 GetForwardDirection() const;
		const glm::vec3& GetPosition() const { return m_Position; }
		glm::quat GetOrientation() const;

		float GetPitch() const { return m_Pitch; }
		float GetYaw() const { return m_Yaw; }

	private:
		void UpdateProjection();
		void UpdateView();
		void UpdateRayDirections();

		bool OnMouseScroll(MouseScrolledEvent& e);

		void MousePan(const glm::vec2& delta);
		void MouseRotate(const glm::vec2& delta);
		void MouseZoom(float delta);

		glm::vec3 CalculatePosition() const;

		std::pair<float, float> PanSpeed() const;
		float RotationSpeed() const;
		float ZoomSpeed() const;

	private:
		float m_FOV = 45.0f, m_AspectRatio = 1.778f, m_NearClip = 0.1f, m_FarClip = 1000.0f;

		glm::mat4 m_Projection = glm::mat4(1.0f);
		glm::mat4 m_View = glm::mat4(1.0f);
		glm::mat4 m_InverseProjection = glm::mat4(1.0f);
		glm::mat4 m_InverseView = glm::mat4(1.0f);

		// Cached ray directions
		std::vector<glm::vec3> m_RayDirections;

		glm::vec3 m_Position = { 0.0f, 0.0f, 4.0f };
		glm::vec3 m_FocalPoint = { 0.0f, 0.0f, 1.0f };

		glm::vec2 m_InitialMousePosition = { 0.0f, 0.0f };

		float m_Distance = 2.0f;
		float m_Pitch = glm::radians(0.0f), m_Yaw = glm::radians(0.0f);

		float m_ViewportWidth = 1920, m_ViewportHeight = 1080;
	};

}