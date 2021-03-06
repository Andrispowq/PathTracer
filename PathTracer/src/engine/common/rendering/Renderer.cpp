#include "Includes.hpp"
#include "Renderer.h"

#include "engine/core/node/component/renderer/RenderableComponent.h"

#include "engine/core/node/component/light/Light.h"

#include "engine/core/resources/AssembledAssetManager.h"

namespace Prehistoric
{
	Renderer::Renderer(Window* window, Camera* camera, AssembledAssetManager* manager)
		: window(window), camera(camera), manager(manager), wireframeMode(false)
	{
	}

	void register_model(std::unordered_map<Pipeline*, std::vector<RenderableComponent*>>& map, RenderableComponent* renderable)
	{
		Pipeline* pipeline = renderable->getPipeline();

		if (map.find(pipeline) != map.end())
		{
			auto& renderables = map[pipeline];
			renderables.push_back(renderable);
		}
		else
		{
			std::vector<RenderableComponent*> renderers = { renderable };
			map.insert(std::make_pair(pipeline, renderers));
		}
	}

	void Renderer::AddModel(RenderableComponent* renderable)
	{
		switch (renderable->getPriority())
		{
		case RenderPriority::_3D:
			register_model(models_3d, renderable);
			break;
		case RenderPriority::_TRANSPARENCY:
			register_model(models_transparency, renderable);
			break;
		case RenderPriority::_2D:
			register_model(models_2d, renderable);
			break;
		default:
			break;
		}
	}

	void Renderer::AddLight(Light* light)
	{
		lights.push_back(light);
	}
};
