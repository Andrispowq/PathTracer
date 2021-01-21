#ifndef SCENE_H
#define SCENE_H

#include "engine/core/node/component/renderer/RendererComponent.h"
#include "world/WorldLoader.h"

#include "engine/core/modules/terrain/Terrain.h"

#include "platform/vulkan/rendering/pipeline/VKPipeline.h"
#include "platform/vulkan/buffer/VKMeshVertexBuffer.h"
#include "platform/vulkan/texture/VkTexture.h"
#include "platform/vulkan/rendering/shaders/pbr/VKPBRShader.h"

#include "engine/core/modules/gui/GUIElement.h"
#include "engine/core/modules/gui/button/GUIButton.h"
#include "engine/core/modules/gui/slider/GUISlider.h"

#include "engine/core/modules/atmosphere/Atmosphere.h"

#include "engine/core/resources/AssembledAssetManager.h"

#include "engine/core/modules/environmentMapRenderer/EnvironmentMapRenderer.h"
#include "engine/core/node/component/audio/AudioComponent.h"

namespace Prehistoric
{
	/*
		TODO: the scene will have a function later on, but it is just some legacy code right now
	*/
	class Scene
	{
	public:
		Scene(const std::string& name, GameObject* root, Window* window, Camera* camera, AssembledAssetManager* manager) 
			: name(name), root(root), window(window), camera(camera), manager(manager) {}
		virtual ~Scene() {}

		std::string getName() const { return name; }
		GameObject* getSceneRoot() const { return root; }

		Scene(const Scene&) = default;
	protected:
		std::string name = "";
		
		GameObject* root;
		Window* window;
		Camera* camera;
		AssembledAssetManager* manager;
	};
};

#endif