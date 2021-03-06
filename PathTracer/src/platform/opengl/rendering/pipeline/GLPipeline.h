#ifndef GL_PIPELINE_H
#define GL_PIPELINE_H

#include "engine/common/rendering/pipeline/Pipeline.h"

#include <glad/glad.h>

#include "engine/common/framework/Window.h"

namespace Prehistoric
{
	class GLPipeline : public Pipeline
	{
	public:
		GLPipeline(Window* window, AssetManager* manager, ShaderHandle shader);
		virtual ~GLPipeline() {}

		virtual void BindPipeline(CommandBuffer* buffer) const override;
		virtual void RenderPipeline() const override;
		virtual void UnbindPipeline() const override;
	};
};

#endif