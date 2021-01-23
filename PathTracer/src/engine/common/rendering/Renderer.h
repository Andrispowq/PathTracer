#ifndef RENDERER_H
#define RENDERER_H

#include "engine/common/framework/Window.h"
#include "engine/core/node/movement/Camera.h"

#include "engine/common/rendering/pipeline/Pipeline.h"

namespace Prehistoric
{
	class RenderableComponent;
	class AssembledAssetManager;

	class Renderer
	{
	public:
		Renderer(Window* window, Camera* camera, AssembledAssetManager* manager);
		virtual ~Renderer() {}

		virtual void PrepareRendering() = 0;
		virtual void EndRendering() = 0;

		virtual void Render() = 0;

		virtual CommandBuffer* getDrawCommandBuffer() const { return nullptr; }

		virtual Texture* getOutputTexture() const { return nullptr; }

		void AddModel(RenderableComponent* renderable);

		Window* getWindow() const { return window; }
		Camera* getCamera() const { return camera; }

		inline bool isWireframeMode() const { return wireframeMode; }
		inline void setWireframeMode(bool wire) { this->wireframeMode = wire; }

	protected:
		AssembledAssetManager* manager;

		Window* window;
		Camera* camera;

		//The rendering context
		std::unordered_map<Pipeline*, std::vector<RenderableComponent*>> models_3d;
		std::unordered_map<Pipeline*, std::vector<RenderableComponent*>> models_transparency;
		std::unordered_map<Pipeline*, std::vector<RenderableComponent*>> models_2d;

		bool wireframeMode;
	};
};

#endif