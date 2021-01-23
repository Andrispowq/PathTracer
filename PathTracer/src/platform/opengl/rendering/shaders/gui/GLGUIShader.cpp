#include "Includes.hpp"
#include "GLGUIShader.h"

namespace Prehistoric
{
	GLGUIShader::GLGUIShader()
		: GLShader()
	{
		AddShader(ResourceLoader::LoadShaderGL("opengl/gui/gui_VS.glsl"), VERTEX_SHADER);
		AddShader(ResourceLoader::LoadShaderGL("opengl/gui/gui_FS.glsl"), FRAGMENT_SHADER);
		CompileShader();

		AddUniform("m_transform");

		AddUniform("colour");
		AddUniform("image");

		AddUniform("exposure");
		AddUniform("gamma");
	}

	void GLGUIShader::UpdateCustomUniforms(Texture* texture, Vector3f colour) const
	{
		SetUniform("m_transform", Matrix4f::Identity());

		SetUniform("colour", colour);
		texture->Bind();
		SetUniformi("image", 0);

		SetUniformf("exposure", EngineConfig::rendererExposure);
		SetUniformf("gamma", EngineConfig::rendererGamma);
	}
};