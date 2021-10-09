#include "Includes.hpp"
#include "GLRenderer.h"

#include "platform/opengl/rendering/pipeline/GLComputePipeline.h"
#include "platform/opengl/rendering/pipeline/GLGraphicsPipeline.h"

#include "platform/opengl/rendering/shaders/tracer/GLPathTracerShader.h"
#include "platform/opengl/rendering/shaders/gui/GLGUIShader.h"

namespace Prehistoric
{
	GLRenderer::GLRenderer(Window* window, Camera* camera, AssembledAssetManager* manager)
		: Renderer(window, camera, manager)
	{
		current_sample = 0;

		AssetManager* man = manager->getAssetManager();

		Vector2u dim = { window->getWidth(), window->getHeight() };
		tracedImage = man->storeTexture(GLTexture::Storage2D(dim.x, dim.y));

		ShaderHandle traceShader = man->loadShader("rayTracing").value();
		ShaderHandle presentShader = man->loadShader("gui").value();

		quadVBO = man->loadVertexBuffer(std::nullopt, "res/models/quad.obj").value();

		rayTracingPipeline = manager->storePipeline(new GLComputePipeline(window, man, traceShader));
		presentPipeline = manager->storePipeline(new GLGraphicsPipeline(window, man, presentShader, quadVBO));

		static_cast<GLComputePipeline*>(rayTracingPipeline.pointer)->setInvocationSize({ dim.x / 32, dim.y / 32, 1 });
		static_cast<GLComputePipeline*>(rayTracingPipeline.pointer)->addTextureBinding(0, tracedImage.pointer, READ_WRITE);
	}

	GLRenderer::~GLRenderer()
	{
	}

	void GLRenderer::PrepareRendering()
	{
		if (window->isResized())
		{
			PR_PROFILE("Window resize pass");

			uint32_t width = window->getWidth();
			uint32_t height = window->getHeight();

			window->getSwapchain()->SetWindowSize(width, height);

			AssetManager* man = manager->getAssetManager();

			man->removeReference<Texture>(tracedImage.handle);

			static_cast<GLComputePipeline*>(rayTracingPipeline.pointer)->removeTextureBinding(0);
			tracedImage = man->storeTexture(GLTexture::Storage2D(width, height));
			static_cast<GLComputePipeline*>(rayTracingPipeline.pointer)->setInvocationSize({ width / 32, height / 32, 1 });
			static_cast<GLComputePipeline*>(rayTracingPipeline.pointer)->addTextureBinding(0, tracedImage.pointer, WRITE_ONLY);

			camera->setChanged();

			//Recreate the pipelines
			std::vector<Pipeline*> pipes = manager->get<Pipeline>();
			for (Pipeline* pipe : pipes)
			{
				pipe->setViewportSize({ (float)width, (float)height });
				pipe->setScissorSize({ width, height });
				pipe->RecreatePipeline();
			}

			window->setResized(false);
		}
	}

	void GLRenderer::EndRendering()
	{
		window->Render(nullptr);

		//Clear the lists for the next iteration
		models_3d.clear();
		models_transparency.clear();
		models_2d.clear();
	}

	void GLRenderer::Render()
	{
		Vector3f position = camera->getPosition();

		if (camera->isChanged() || (current_sample == 0))
		{
			current_sample = 1;

			Matrix4f invVP = camera->getViewProjectionMatrix().Invert();

			Vector4f r00 = invVP * Vector4f(-1, -1, 1, 1);
			Vector4f r01 = invVP * Vector4f(-1, 1, 1, 1);
			Vector4f r10 = invVP * Vector4f(1, -1, 1, 1);
			Vector4f r11 = invVP * Vector4f(1, 1, 1, 1);

			ray00 = r00.xyz() / r00.w - position;
			ray01 = r01.xyz() / r01.w - position;
			ray10 = r10.xyz() / r10.w - position;
			ray11 = r11.xyz() / r11.w - position;
		}
		else
		{
			current_sample++;
		}

		rayTracingPipeline->BindPipeline(nullptr);
		static_cast<GLPathTracerShader*>(rayTracingPipeline->getShader())->UpdateUniforms(camera, current_sample);
		rayTracingPipeline->RenderPipeline();
		rayTracingPipeline->UnbindPipeline();

		presentPipeline->BindPipeline(nullptr);
		static_cast<GLGUIShader*>(presentPipeline->getShader())->UpdateCustomUniforms(tracedImage.pointer, Vector3f(-1));
		presentPipeline->RenderPipeline();
		presentPipeline->UnbindPipeline();
	}
};
