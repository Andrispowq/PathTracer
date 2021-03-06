#ifndef LIGHT_H
#define LIGHT_H

#include "engine/core/node/component/Component.h"

namespace Prehistoric
{
	class Light : public Component
	{
	public:
		Light(const Vector3f& colour = 0, const Vector3f& intensity = 1);
		virtual ~Light() {}

		virtual void PreRender(Renderer* renderer) override;

		inline Vector3f getColour() const { return colour; }
		inline Vector3f getIntensity() const { return intensity; }

		inline void setColour(const Vector3f& colour) { this->colour = colour; }
		inline void setIntensity(const Vector3f& intensity) { this->intensity = intensity; }

		static ComponentType getStaticComponentType() { return ComponentType::LightComponent; }

	private:
		Vector3f colour;
		Vector3f intensity;
	};
};

#endif