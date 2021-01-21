#include "Includes.hpp"
#include "Capabilities.h"

#include "platform/opengl/capabilities/GLCapabilities.h"

namespace Prehistoric
{
	Capabilities* Capabilities::instance = nullptr;

	Capabilities* Capabilities::getInstance()
	{
		if (instance == nullptr)
		{
			if (FrameworkConfig::api == OpenGL)
			{
				instance = new GLCapabilities();
			}
		}

		return instance;
	}
};