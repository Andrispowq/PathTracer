#ifndef GL_IRRADIANCE_SHADER_H
#define GL_IRRADIANCE_SHADER_H

#include "platform/opengl/rendering/shaders/GLShader.h"

#include <glad/glad.h>

#include "engine/common/texture/Texture.h"

namespace Prehistoric
{
	class GLIrradianceShader : public GLShader
	{
	public:
		GLIrradianceShader();
		virtual ~GLIrradianceShader() {}

		void UpdateUniforms(Texture* texture) const;
	};
};

#endif