#ifndef GL_PBR_SHADER_H
#define GL_PBR_SHADER_H

#include "platform/opengl/rendering/shaders/GLShader.h"

#include <glad/glad.h>

#include "engine/core/node/GameObject.h"
#include "engine/core/node/movement/Camera.h"
#include "engine/core/node/component/renderer/RendererComponent.h"
#include "engine/core/model/material/Material.h"
#include "platform/opengl/texture/GLTexture.h"

#include "engine/core/config/EngineConfig.h"

namespace Prehistoric
{
	class GLPBRShader : public GLShader
	{
	public:
		GLPBRShader();
		virtual ~GLPBRShader() {}

		virtual void UpdateShaderUniforms(Camera* camera, const std::vector<Light*>& lights, uint32_t instance_index = 0) const override;
		virtual void UpdateObjectUniforms(GameObject* object, uint32_t instance_index = 0) const override;

	private:
		int m_transform;

		int albedoMap;
		int normalMap;
		int mrotMap;
		int emissionMap;

		int colour;
		int usesNormalMap;
		int mrot;
		int emission;
	};
};

#endif