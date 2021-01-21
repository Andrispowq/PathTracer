#ifndef GL_BRDF_INTEGRATE_SHADER_H
#define GL_BRDF_INTEGRATE_SHADER_H

#include "platform/opengl/rendering/shaders/GLShader.h"

#include <glad/glad.h>

#include "engine/common/texture/Texture.h"

namespace Prehistoric
{
	class GLBRDFIntegrateShader : public GLShader
	{
	public:
		GLBRDFIntegrateShader();
		virtual ~GLBRDFIntegrateShader() {}
	};
};

#endif