#ifndef ENGINE_CONFIG_H
#define ENGINE_CONFIG_H

#include <fstream>

#include "engine/core/util/Util.h"
#include "engine/core/util/math/Math.h"

namespace Prehistoric
{
    namespace EngineConfig
    {
        void LoadConfig(const std::string& path);

        extern uint32_t lightsMaxNumber;
        extern float rendererGamma;
        extern float rendererExposure;
        extern int rendererHighDetailRange;
        extern float rendererNearPlane;
        extern float rendererFarPlane;
    };
};

#endif