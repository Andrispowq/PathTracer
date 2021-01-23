#include "Includes.hpp"
#include "GLPathTracerShader.h"

#include "engine/core/config/EnvironmentMapConfig.h"

namespace Prehistoric
{
	GLPathTracerShader::GLPathTracerShader()
	{
		AddShader(ResourceLoader::LoadShaderGL("opengl/tracer/path_tracer_CS.glsl"), COMPUTE_SHADER);

		CompileShader();

		AddUniform("environment");

		AddUniform("eye");
		AddUniform("ray00");
		AddUniform("ray01");
		AddUniform("ray10");
		AddUniform("ray11");

		AddUniform("time_");
		AddUniform("current_spp_");
		AddUniform("blendFactor");
	}

	void GLPathTracerShader::UpdateUniforms(Vector3f eye, Vector3f ray00, Vector3f ray01, Vector3f ray10, Vector3f ray11, int spp) const
	{
		EnvironmentMapConfig::environmentMap->Bind();
		SetUniformi("environment", 0);

		SetUniform("eye", eye);
		SetUniform("ray00", ray00);
		SetUniform("ray01", ray01);
		SetUniform("ray10", ray10);
		SetUniform("ray11", ray11);

		float blend = float(spp) / float(spp + 1);
		SetUniformf("time_", (float)Time::getTimeFromStart());
		SetUniformi("current_spp_", spp);
		SetUniformf("blendFactor", blend);
	}
};
