#ifndef SCENE_H
#define SCENE_H

#include "engine/core/resources/AssembledAssetManager.h"

namespace Prehistoric
{
	/*
		TODO: the scene will have a function later on, but it is just some legacy code right now
	*/
	class Scene
	{
	public:
		Scene(const std::string& name, GameObject* root, Window* window, Camera* camera, AssembledAssetManager* manager) 
			: name(name), root(root), window(window), camera(camera), manager(manager) {}
		virtual ~Scene() {}

		std::string getName() const { return name; }
		GameObject* getSceneRoot() const { return root; }

		Scene(const Scene&) = default;
	protected:
		std::string name = "";
		
		GameObject* root;
		Window* window;
		Camera* camera;
		AssembledAssetManager* manager;
	};
};

#endif