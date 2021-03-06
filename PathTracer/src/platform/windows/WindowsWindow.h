#ifndef WINDOWS_WINDOW_H
#define WINDOWS_WINDOW_H

#include "engine/common/framework/Window.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "engine/core/resources/loaders/TextureLoader.h"
#include "engine/core/config/FrameworkConfig.h"
#include "platform/Platform.h"

#include "Includes.hpp"
#include "engine/core/events/Event.h"

#include "engine/core/events/ApplicationEvent.h"

namespace Prehistoric
{
	class WindowsWindow : public Window
	{
	public:
		WindowsWindow() : Window(), window(nullptr) {}

		virtual ~WindowsWindow();

		virtual bool Create() override;
		virtual bool ShouldClose() const override;
		virtual void Input() override;
		virtual void Render(CommandBuffer* buffer) const override;

		virtual void OnEvent(Event& event) override;

		virtual void SetFullscreen(bool fullscreen) override;
		virtual bool IsFullscreen() const override;

		virtual void* getWindowHandle() const override { return window; }
	private:
		bool initGLFW() const;
	private:
		int oldWidth, oldHeight;
		int oldX, oldY;

		GLFWwindow* window;
		GLFWmonitor* monitor;
	};
};

#endif
