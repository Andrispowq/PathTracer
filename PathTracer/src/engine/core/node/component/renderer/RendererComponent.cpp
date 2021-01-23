#include "Includes.hpp"
#include "RendererComponent.h"
#include "engine/common/rendering/pipeline/Pipeline.h"
#include "engine/core/engines/RenderingEngine.h"
#include "engine/core/node/GameObject.h"
#include "engine/core/model/material/Material.h"

namespace Prehistoric
{
	RendererComponent::RendererComponent(Window* window, AssembledAssetManager* manager, PipelineHandle pipeline, MaterialHandle material)
		: RenderableComponent(window, manager, pipeline), material(material)
	{
		type = ComponentType::RendererComponent;

		manager->addReference<Material>(material.handle);
	}

	RendererComponent::RendererComponent(Window* window, AssembledAssetManager* manager)
		: RenderableComponent(window, manager)
	{
		type = ComponentType::RendererComponent;
	}

	RendererComponent::~RendererComponent()
	{
		if(material.pointer != nullptr)
			manager->removeReference<Material>(material.handle);
	}

	void RendererComponent::PreRender(Renderer* renderer)
	{
		if(pipeline.pointer != nullptr)
			renderer->AddModel(this);
	}

	void RendererComponent::Render(Renderer* renderer) const
	{
		pipeline->BindPipeline(renderer->getDrawCommandBuffer());
		pipeline->RenderPipeline();
		pipeline->UnbindPipeline();
	}

	void RendererComponent::BatchRender(uint32_t instance_index) const
	{
		pipeline->RenderPipeline();
	}
};
