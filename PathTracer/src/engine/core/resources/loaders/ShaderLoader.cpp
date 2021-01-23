#include "Includes.hpp"
#include "ShaderLoader.h"

#include "platform/opengl/rendering/shaders/gui/GLGUIShader.h"
#include "platform/opengl/rendering/shaders/tracer/GLPathTracerShader.h"

namespace Prehistoric
{
    void* ShaderLoader::LoadResourceInternal(const std::string& path, Extra* extra)
    {
        Shader* shader = nullptr;

        if (FrameworkConfig::api == OpenGL)
        {
            if (path == "gui")
            {
                shader = new GLGUIShader();
            }
            else if (path == "rayTracing")
            {
                shader = new GLPathTracerShader();
            }
        }

        return shader;
    }
};
