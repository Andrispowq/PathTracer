#ifndef DEVICE_PROPERTIES
#define DEVICE_PROPERTIES

#include "engine/common/capabilities/Capabilities.h"

namespace Prehistoric
{
	class DeviceProperties
	{
	public:
		void ListProperties(Capabilities& capabilities) const;
	};
};

#endif