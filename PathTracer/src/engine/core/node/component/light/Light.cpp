#include "Includes.hpp"
#include "Light.h"

#include "engine/common/rendering/Renderer.h"

namespace Prehistoric
{
	Light::Light(const Vector3f& colour, const Vector3f& intensity)
		: colour(colour), intensity(intensity)
	{
		type = ComponentType::LightComponent;
	}

	void Light::PreRender(Renderer* renderer)
	{
		renderer->AddLight(this);
	}
};