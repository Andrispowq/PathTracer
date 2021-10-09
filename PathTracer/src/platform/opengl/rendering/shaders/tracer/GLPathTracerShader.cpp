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

		AddUniform("Projection");
		AddUniform("InvProjection");
		AddUniform("NearFar");
		AddUniform("View");
		AddUniform("InvView");
		AddUniform("ProjectionView");
		AddUniform("ViewPos");
		AddUniform("ViewDir");

		AddUniform("current_spp");
		AddUniform("blendFactor");
	}

	void GLPathTracerShader::UpdateUniforms(Camera* camera, int spp) const
	{
		EnvironmentMapConfig::environmentMap->Bind();
		SetUniformi("environment", 0);

		SetUniform("Projection", camera->getProjectionMatrix());
		SetUniform("InvProjection", camera->getProjectionMatrix().Invert());
		SetUniform("NearFar", Vector2f(EngineConfig::rendererNearPlane, EngineConfig::rendererFarPlane));
		SetUniform("View", camera->getViewMatrix());
		SetUniform("InvView", camera->getViewMatrix().Invert());
		SetUniform("ProjectionView", camera->getViewProjectionMatrix());
		SetUniform("ViewPos", camera->getPosition());
		SetUniform("ViewDir", camera->getForward());

		float blend = float(spp) / float(spp + 1);
		SetUniformi("current_spp", spp);
		SetUniformf("blendFactor", blend);
	}
};
