#ifndef WINDOWS_INPUT_H
#define WINDOWS_INPUT_H

#include "engine/common/framework/Input.h"

#include <GLFW/glfw3.h>

#include "engine/core/util/math/Math.h"

#include "engine/core/events/KeyEvent.h"
#include "engine/core/events/MouseEvent.h"
#include "engine/core/events/JoystickEvent.h"

namespace Prehistoric
{
	class WindowsInput : public Input
	{
	public:
		virtual bool Init(Window* window) const override;
		virtual bool Update() override;

		virtual void OnEvent(Event& event) override;

		virtual void setCursorPositionOnScreen(Window* window, const Vector2f& cursorPosition) override;
	};
};

#endif
