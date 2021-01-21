#ifndef APPLICATION_H
#define APPLICATION_H

#include "engine/core/events/Event.h"

#include "engine/core/layers/LayerStack.h"

#include "engine/core/CoreEngine.h"
#include "engine/core/ImGUI/ImGUILayer.h"

namespace Prehistoric
{
	class Application
	{
	public:
		Application();
		virtual ~Application();

		void Run();
		void Stop();

		void OnEvent(Event& e);

		void PushLayer(Layer* layer);
		void PushOverlay(Layer* layer);

		CoreEngine* getEngineLayer() { return engineLayer; }
		ImGuiLayer* getImGuiLayer() { return imGUILayer; }

		int getLastFPS() const { return last_fps; }
		double getFrameTime() const { return frameTime * 1000; }

		static Application& Get() { return *instance; }
	protected:
		LayerStack layerStack;
		bool running = false;
		double frameTime;
		int last_fps;

		CoreEngine* engineLayer;
		ImGuiLayer* imGUILayer;

		static Application* instance;
	};

	//To be defined in client
	Application* CreateApplication();
};

#endif