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

		void UpdateUniforms(Vector3f eye, Vector3f ray00, Vector3f ray01, Vector3f ray10, Vector3f ray11, int spp) const;
	};
};

#endif