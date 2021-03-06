#ifndef RENDERABLE_COMPONENT_H
#define RENDERABLE_COMPONENT_H

#include "engine/core/node/component/Component.h"

#include "engine/common/rendering/pipeline/Pipeline.h"
#include "engine/common/framework/Window.h"

#include "engine/core/resources/AssembledAssetManager.h"

#include "Includes.hpp"

namespace Prehistoric
{
	class AssembledAssetManager;

	enum class RenderPriority
	{
		_3D = 0,
		_TRANSPARENCY = 1,
		_2D = 2,
		_POST_PROCESSING = 3
	};

	class RenderableComponent : public Component
	{
	public:
		RenderableComponent(Window* window, AssembledAssetManager* manager, PipelineHandle pipeline);
		RenderableComponent(Window* window, AssembledAssetManager* manager);
		virtual ~RenderableComponent();

		virtual void Render(Renderer* renderer) const = 0;
		virtual void BatchRender(uint32_t instance_index = 0) const = 0;

		Pipeline* getPipeline() const { return pipeline.pointer; }
		PipelineHandle getPipelineHandle() const { return pipeline; }

		inline RenderPriority getPriority() const { return priority; }
		inline void setPriority(RenderPriority priority) { this->priority = priority; }

		static ComponentType getStaticComponentType() { return ComponentType::RenderableComponent; }

		RenderableComponent(const RenderableComponent&) = delete;
		RenderableComponent(const RenderableComponent&&) = delete;
		RenderableComponent& operator=(RenderableComponent) = delete;
	protected:
		AssembledAssetManager* manager;

		Window* window;

		PipelineHandle pipeline;
		RenderPriority priority;
	};
};

#endif