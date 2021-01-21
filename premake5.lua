workspace "PathTracer"
    architecture "x64"
    startproject "TesterProject"

    configurations
    {
        "Debug",
        "Release"
    }

    flags
	{
		"MultiProcessorCompile"
	}

outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

IncludeDir = {}
IncludeDir["GLFW"] = "%{wks.location}/PathTracer/vendor/GLFW/include"
IncludeDir["GLAD"] = "%{wks.location}/PathTracer/vendor/GLAD/include"
IncludeDir["ImGUI"] = "%{wks.location}/PathTracer/vendor/ImGUI"
IncludeDir["STB"] = "%{wks.location}/PathTracer/Dependencies/include/stb"
IncludeDir["tinyobj"] = "%{wks.location}/PathTracer/Dependencies/include/tinyobjloader"

group "Dependencies"
    include "PathTracer/vendor/GLFW"
    include "PathTracer/vendor/GLAD"
    include "PathTracer/vendor/ImGUI"
group ""

project "PathTracer"
    location "PathTracer"
    kind "StaticLib"
    language "C++"
    cppdialect "C++17"
    staticruntime "off"
    
    targetdir ("bin/" .. outputdir .. "/%{prj.name}")
    objdir ("bin-int/" .. outputdir .. "/%{prj.name}")

    pchheader "Includes.hpp"
    pchsource "%{prj.location}/src/Includes.cpp"

    files
    {
        "%{prj.name}/src/**.h",
        "%{prj.name}/src/**.hpp",
        "%{prj.name}/src/**.cpp"
    }

    defines
	{
		"_CRT_SECURE_NO_WARNINGS",
		"GLFW_INCLUDE_NONE"
    }

    includedirs
    {
        "%{IncludeDir.GLFW}",
        "%{IncludeDir.GLAD}",
        "%{IncludeDir.ImGUI}",
        "%{IncludeDir.STB}",
        "%{IncludeDir.tinyobj}",
        "%{prj.location}/src"
    }

    links
    {
        "opengl32.lib",
        "GLFW",
        "GLAD",
        "ImGUI"
    }

    filter "system:windows"
        systemversion "latest"

        defines
        {
            "PR_FAST_MATH"
        }
    
    filter "configurations:Debug"
        defines "PR_DEBUG"
        runtime "Debug"
        symbols "on"
    
    filter "configurations:Release"
        defines "PR_RELEASE"
        runtime "Release"
        optimize "on"

project "TesterProject"
    location "TesterProject"
    kind "ConsoleApp"
    language "C++"
    cppdialect "C++17"
    staticruntime "off"
    
    targetdir ("bin/" .. outputdir .. "/%{prj.name}")
    objdir ("bin-int/" .. outputdir .. "/%{prj.name}")

    files
    {
        "%{prj.name}/src/**.h",
        "%{prj.name}/src/**.hpp",
        "%{prj.name}/src/**.cpp"
    }

    defines
    {
        "_CRT_SECURE_NO_WARNINGS",
        "GLFW_INCLUDE_NONE"
    }

    includedirs
    {
        "%{IncludeDir.GLFW}",
        "%{IncludeDir.GLAD}",
        "%{IncludeDir.ImGUI}",
        "%{IncludeDir.STB}",
        "%{IncludeDir.tinyobj}",
        "%{prj.location}/src",
        "%{wks.location}/PathTracer/src"
    }

    links
    {
        "PathTracer"
    }

    filter "system:windows"
        systemversion "latest"

        defines
        {
            "PR_FAST_MATH"
        }
    
    filter "configurations:Debug"
        defines "PR_DEBUG"
        runtime "Debug"
        symbols "on"
    
    filter "configurations:Release"
        defines "PR_RELEASE"
        runtime "Release"
        optimize "on"