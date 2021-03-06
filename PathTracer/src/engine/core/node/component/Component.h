#ifndef COMPONENT_H
#define COMPONENT_H

#include "engine/core/node/Node.h"

namespace Prehistoric
{
	class GameObject;

	enum class ComponentType
	{
		General,
		AudioComponent,
		RenderableComponent,
		RendererComponent,
		LightComponent
	};

	class Component
	{
	public:
		Component() : parent(nullptr), type(ComponentType::General) {}
		virtual ~Component() {}

		virtual void PreUpdate(CoreEngine* engine) {}
		virtual void PreRender(Renderer* renderer) {}

		inline GameObject* getParent() const { return parent; }
		inline void setParent(GameObject* parent) { this->parent = parent; }

		virtual ComponentType getComponentType() const { return type; };
		static ComponentType getStaticComponentType() { return ComponentType::General; };
	protected:
		GameObject* parent;
		ComponentType type;
	};
};

#endif