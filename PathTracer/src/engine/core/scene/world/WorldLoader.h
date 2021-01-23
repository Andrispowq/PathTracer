#ifndef WORLD_LOADER_H
#define WORLD_LOADER_H

#include <fstream>

#include "engine/core/node/GameObject.h"
#include "engine/common/buffer/MeshVertexBuffer.h"
#include "engine/core/model/material/Material.h"
#include "engine/common/rendering/shaders/Shader.h"
#include "engine/core/node/component/renderer/RendererComponent.h"
#include "engine/core/model/obj/OBJLoader.h"

#include "platform/opengl/rendering/pipeline/GLGraphicsPipeline.h"

#include "engine/core/resources/AssembledAssetManager.h"

#include "Includes.hpp"

namespace Prehistoric
{
	class WorldLoader
	{
	public:
		WorldLoader() {}
		virtual ~WorldLoader() {}

		void LoadWorld(const std::string& worldFile, GameObject* root, Window* window, AssembledAssetManager* manager);
	private:
		std::string directoryModels;
		std::string directoryTextures;

		//Rendering stuff
		std::unordered_map<std::string, TextureHandle> textures;
		std::vector<std::string> textureNames;
		std::unordered_map<std::string, MaterialHandle> materials;

		std::unordered_map<std::string, VertexBufferHandle> models;
		std::unordered_map<std::string, ShaderHandle> shaders;
		std::unordered_map<std::string, PipelineHandle> pipelines;
	};
};

#endif