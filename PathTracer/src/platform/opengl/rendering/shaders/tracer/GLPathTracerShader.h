#ifndef GL_PATH_TRACER_SHADER_H
#define GL_PATH_TRACER_SHADER_H

#include "platform/opengl/rendering/shaders/GLShader.h"

#include <glad/glad.h>

namespace Prehistoric
{
	class GLPathTracerShader : public GLShader
	{
	public:
		GLPathTracerShader();
		virtual ~GLPathTracerShader() {}

		void UpdateUniforms(Camera* camera, int spp) const;
	};
};

#endif