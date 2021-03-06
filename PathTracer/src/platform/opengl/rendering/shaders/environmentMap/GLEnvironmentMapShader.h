#ifndef GL_ENVIROMENT_MAP_SHADER_H
#define GL_ENVIROMENT_MAP_SHADER_H

#include "platform/opengl/rendering/shaders/GLShader.h"

#include <glad/glad.h>

#include "engine/common/texture/Texture.h"

namespace Prehistoric
{
	class GLEnvironmentMapShader : public GLShader
	{
	public:
		GLEnvironmentMapShader();
		virtual ~GLEnvironmentMapShader() {}

		void UpdateUniforms(Vector2f equirectMapRes) const;
	};
};

#endif