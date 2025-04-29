#pragma once

#include "Core/KeyCodes.h"
#include "Core/MouseCodes.h"

namespace RayMagic {

	class Input
	{
	public:
		static bool IsKeyPressed(KeyCode keycode);

		static bool IsMouseButtonPressed(MouseCode button);
		static std::pair<float, float> GetMousePosition();
		static float GetMouseX();
		static float GetMouseY();
	};

}