workspace "eon-gl"
    architecture "x64"
    configurations {"Debug", "Release"}
    startproject "eon-gl"

    
outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

includeDir = {}
includeDir["GLFW"] = "vendor/GLFW/include"
includeDir["GLAD"] = "vendor/GLAD/include"
includeDir["spdlog"] = "vendor/spdlog/include"

include "vendor/GLFW"
include "vendor/GLAD"

project "eon-gl"
    kind "ConsoleApp"
    language "C++"
    cppdialect "C++17"
    staticruntime "on"

    targetdir ("build/bin/" .. outputdir .. "/%{prj.name}")
    objdir ("build/bin-int/" .. outputdir .. "/%{prj.name}")

    files
    {
        "src/**.h",
        "src/**.cpp"
    }

    defines
    {
        "_CRT_SECURE_NO_WARNINGS"
    }

    includedirs
    {
        "src",
        "%{includeDir.GLFW}",
        "%{includeDir.GLAD}",
        "%{includeDir.spdlog}"
    }

    links 
    { 
        "GLFW",
        "GLAD",
        "opengl32.lib"
    }

    filter "system:windows"
        systemversion "latest"

    filter "configurations:Debug"
        defines "EON_DEBUG"
        runtime "Debug"
        symbols "on"

    filter "configurations:Release"
        defines "EON_RELEASE"
        runtime "Release"
        optimize "on"




