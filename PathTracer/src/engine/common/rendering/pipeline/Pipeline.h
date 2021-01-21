#ifndef PIPELINE_H
#define PIPELINE_H

#include "engine/common/buffer/VertexBuffer.h"
#include "engine/common/rendering/shaders/Shader.h"

#include "engine/core/util/math/Math.h"
#include "engine/common/framework/Window.h"

#include "engine/core/resources/AssetManager.h"

namespace Prehistoric
{
	class Pipeline
	{
	public:
		Pipeline(Window* window, AssetManager* manager, ShaderHandle shader);
		Pipeline() : samples(0) {}

		virtual ~Pipeline();

		virtual void BindPipeline(CommandBuffer* buffer) const = 0;
		virtual void RenderPipeline() const = 0;
		virtual void UnbindPipeline() const = 0;

		virtual void RecreatePipeline() {};

		Vector2f getViewportStart() const { return viewportStart; }
		Vector2f getViewportSize() const { return viewportSize; }
		Vector2u getScissorStart() const { return scissorStart; }
		Vector2u getScissorSize() const { return scissorSize; }
		int getSamples() const { return samples; }

		Shader* getShader() const { return shader.pointer; }
		ShaderHandle getShaderHandle() const { return shader; }

		void setViewportStart(const Vector2f& viewportStart) { this->viewportStart = viewportStart; }
		void setViewportSize(const Vector2f& viewportSize) { this->viewportSize = viewportSize; }
		void setScissorStart(const Vector2u& scissorStart) { this->scissorStart = scissorStart; }
		void setScissorSize(const Vector2u& scissorSize) { this->scissorSize = scissorSize; }
		void setSamples(int samples) { this->samples = samples; }

	public:
		Window* window;
		AssetManager* manager;

		ShaderHandle shader;

		Vector2f viewportStart;
		Vector2f viewportSize;
		Vector2u scissorStart;
		Vector2u scissorSize;

		int samples;

	protected:
		mutable CommandBuffer* buffer; //The current commandbuffer
	};
};

#endif