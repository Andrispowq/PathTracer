#include "TesterApp.h"

#include "engine/core/resources/AssembledAssetManager.h"

#include "engine/core/modules/environmentMapRenderer/EnvironmentMapRenderer.h"
#include "engine/core/node/component/renderer/RendererComponent.h"

#include "platform/opengl/rendering/pipeline/GLGraphicsPipeline.h"

TesterApp::TesterApp()
{
	using namespace Prehistoric;

	//GameObject* audio = new GameObject();
	//audio->AddComponent(AUDIO_COMPONENT, new AudioComponent("res/sounds/_Closer.wav", 75.0f));
	//audio->GetComponent<AudioComponent>()->PreUpdate(engineLayer);

	//engineLayer->getRootObject()->AddChild("audio", audio);

	//engineLayer->getAudioEngine()->Update(0.0f);

	//Load in the environment map
	if (FrameworkConfig::api == OpenGL)
	{
		{
			PR_PROFILE("Environment map generation - BRDF Look-up Table");
			EnvironmentMapRenderer::instance = new EnvironmentMapRenderer(engineLayer->getRenderingEngine()->getWindow(), engineLayer->getAssetManager());
		}

		{
			PR_PROFILE("Environment map generation - Cubemap, Irradiance, Prefilter map");
			EnvironmentMapRenderer::instance->GenerateEnvironmentMap();
			EnvironmentMapRenderer::instance->lodRenderedMap = 0.0f;
		}
	}
}

TesterApp::~TesterApp()
{
}
