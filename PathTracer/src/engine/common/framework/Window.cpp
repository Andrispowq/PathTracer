#include "Includes.hpp"
#include "Window.h"

namespace Prehistoric
{
	Window::Window()
		: swapchain(nullptr), context(nullptr), resized(false)
	{
		this->width = FrameworkConfig::windowWidth;
		this->height = FrameworkConfig::windowHeight;
		this->title = FrameworkConfig::windowName.c_str();
		this->fullscreen = FrameworkConfig::windowFullScreen;
		this->closed = true;
	}

	Window::~Window()
	{
		delete swapchain.release();
		delete context.release();
	}
};
