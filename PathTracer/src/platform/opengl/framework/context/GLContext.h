#ifndef GL_CONTEXT_H
#define GL_CONTEXT_H

#include "engine/common/framework/context/Context.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "engine/common/framework/Window.h"

namespace Prehistoric
{
	class GLContext : public Context
	{
	public:
		GLContext(Window* window);
		virtual ~GLContext() {}
	};
};

#endif