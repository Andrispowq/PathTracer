#include "Includes.hpp"
#include "RenderableComponent.h"
#include "engine/core/model/material/Material.h"
#include "engine/common/rendering/pipeline/Pipeline.h"
#include "engine/common/rendering/pipeline/GraphicsPipeline.h"
#include "engine/common/rendering/pipeline/ComputePipeline.h"

#include "engine/core/resources/AssembledAssetManager.h"

namespace Prehistoric
{
	RenderableComponent::RenderableComponent(Window* window, AssembledAssetManager* manager, PipelineHandle pipeline)
		: priority(RenderPriority::_3D), window(window), manager(manager)
	{
		type = ComponentType::RenderableComponent;

		this->window = window;
		this->manager = manager;

		this->pipeline = pipeline;
		manager->addReference<Pipeline>(pipeline.handle);
	}

	RenderableComponent::RenderableComponent(Window* window, AssembledAssetManager* manager)
		: priority(RenderPriority::_3D), window(window), manager(manager)
	{
		type = ComponentType::RendererComponent;
	}

	RenderableComponent::~RenderableComponent()
	{
		manager->removeReference<Pipeline>(pipeline.handle);
	}
};
