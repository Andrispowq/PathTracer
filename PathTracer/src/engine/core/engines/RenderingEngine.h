#ifndef RENDERING_ENGINE_H
#define RENDERING_ENGINE_H

#include "Engine.h"

#include "engine/core/node/movement/Camera.h"
#include "engine/core/resources/AssembledAssetManager.h"

#include "engine/common/framework/Window.h"
#include "engine/common/rendering/Renderer.h"

namespace Prehistoric
{
	struct Statistics
	{
		size_t drawcalls;
		size_t vertexCount;
		size_t indexCount;
	};

	class RenderingEngine : public Engine
	{
	public:
		RenderingEngine();
		virtual ~RenderingEngine();

		void Init(AssembledAssetManager* manager);

		virtual void OnEvent(Event& event) override;

		virtual void Update(float delta) override;
		void Render();

		inline Window* getWindow() const { return window.get(); }
		inline Camera* getCamera() const { return camera.get(); }

		inline Renderer* getRenderer() const { return renderer.get(); }

		static Statistics& getStats() { return statistics; }

		RenderingEngine(const RenderingEngine& engine) = delete;
		RenderingEngine operator=(const RenderingEngine& engine) = delete;
	private:
		std::unique_ptr<Window> window;
		std::unique_ptr<Camera> camera;

		std::unique_ptr<Renderer> renderer;

		static Statistics statistics;
	};
};

#endif