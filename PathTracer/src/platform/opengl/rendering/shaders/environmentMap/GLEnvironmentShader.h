#ifndef GL_ENVIROMENT_SHADER_H
#define GL_ENVIROMENT_SHADER_H

#include "platform/opengl/rendering/shaders/GLShader.h"

#include <glad/glad.h>

#include "engine/common/texture/Texture.h"

namespace Prehistoric
{
	class GLEnvironmentShader : public GLShader
	{
	public:
		GLEnvironmentShader();
		virtual ~GLEnvironmentShader() {}

		void UpdateUniforms(const Matrix4f& projection, const Matrix4f& view, Texture* texture, float lod) const;
	};
};

#endif