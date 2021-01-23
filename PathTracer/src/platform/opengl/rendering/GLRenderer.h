#ifndef GL_RENDERER_H
#define GL_RENDERER_H

#include "engine/common/rendering/Renderer.h"

#include <glad/glad.h>

#include "platform/opengl/rendering/framebuffer/GLFramebuffer.h"

#include "engine/core/resources/AssembledAssetManager.h"

namespace Prehistoric
{
	class GLRenderer : public Renderer
	{
	public:
		GLRenderer(Window* window, Camera* camera, AssembledAssetManager* manager);
		virtual ~GLRenderer();

		virtual void PrepareRendering();
		virtual void EndRendering();

		virtual void Render() override;

	private:
		TextureHandle tracedImage;

		VertexBufferHandle quadVBO;
		PipelineHandle rayTracingPipeline;
		PipelineHandle presentPipeline;

		Vector3f ray00, ray01, ray10, ray11;
		int current_sample;
	};
};

#endif