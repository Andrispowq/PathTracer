#include "Includes.hpp"
#include "GLPBRShader.h"

#include "engine/core/config/EnvironmentMapConfig.h"

namespace Prehistoric
{
	GLPBRShader::GLPBRShader() : GLShader()
	{
		AddShader(ResourceLoader::LoadShaderGL("opengl/pbr/pbr_VS.glsl"), VERTEX_SHADER);
		AddShader(ResourceLoader::LoadShaderGL("opengl/pbr/pbr_FS.glsl"), FRAGMENT_SHADER);
		CompileShader();

		m_transform = glGetUniformLocation(program, "m_transform");
		AddUniform("m_view");
		AddUniform("m_projection");

		AddUniform("cameraPosition");
		AddUniform("highDetailRange");

		albedoMap = glGetUniformLocation(program, (std::string("material.") + ALBEDO_MAP).c_str());
		normalMap = glGetUniformLocation(program, (std::string("material.") + NORMAL_MAP).c_str());
		mrotMap = glGetUniformLocation(program, (std::string("material.") + MROT_MAP).c_str());
		emissionMap = glGetUniformLocation(program, (std::string("material.") + EMISSION_MAP).c_str());

		colour = glGetUniformLocation(program, (std::string("material.") + COLOUR).c_str());
		usesNormalMap = glGetUniformLocation(program, (std::string("material.") + USES_NORMAL_MAP).c_str());
		mrot = glGetUniformLocation(program, (std::string("material.") + MROT).c_str());
		emission = glGetUniformLocation(program, (std::string("material.") + EMISSION).c_str());

		AddUniform("highDetailRange");
	}

	void GLPBRShader::UpdateShaderUniforms(Camera* camera, const std::vector<Light*>& lights, uint32_t instance_index) const
	{
		SetUniform("m_view", camera->getViewMatrix());
		SetUniform("m_projection", camera->getProjectionMatrix());
		SetUniform("cameraPosition", camera->getPosition());
		SetUniformi("highDetailRange", EngineConfig::rendererHighDetailRange);
	}

	void GLPBRShader::UpdateObjectUniforms(GameObject* object, uint32_t instance_index) const
	{
		SetUniform(m_transform, object->getWorldTransform().getTransformationMatrix());

		Material* material = static_cast<RendererComponent*>(object->GetComponent(RENDERER_COMPONENT))->getMaterial();

		material->getTexture(ALBEDO_MAP)->Bind(0);
		SetUniformi(albedoMap, 0);
		material->getTexture(NORMAL_MAP)->Bind(1);
		SetUniformi(normalMap, 1);
		material->getTexture(MROT_MAP)->Bind(2);
		SetUniformi(mrotMap, 2);
		material->getTexture(EMISSION_MAP)->Bind(3);
		SetUniformi(emissionMap, 3);

		SetUniform(colour, material->getVector3f(COLOUR));
		SetUniformi(usesNormalMap, material->exists(NORMAL_MAP));
		SetUniform(mrot, material->getVector4f(MROT));
		SetUniform(emission, material->getVector3f(EMISSION));
	}
};