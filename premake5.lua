workspace "LineDesigner"
architecture "x64"
    configurations { "Debug", "Release" }
    outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

project "Core"
    location "%{prj.name}"
    kind "WindowedApp"
    language "C++"
    targetname "LineDesigner"
    targetdir ("bin/".. outputdir)
    objdir ("%{prj.name}/int/" .. outputdir)
    cppdialect "C++17"
    staticruntime "Off"

    files
    {
        "%{prj.name}/**.h",
        "%{prj.name}/**.c",
        "%{prj.name}/**.hpp"
,        "%{prj.name}/**.cpp"
    }

    includedirs
    {
        "%{prj.name}/include/ImGui",
        "%{prj.name}/include",
        "%{prj.name}/src"
    }

    defines
    {
        "SFML_STATIC",
        "YAML_CPP_STATIC_DEFINE"
    }

    libdirs "%{prj.name}/lib"

    links
    {
        "opengl32",
        "winmm",
        "gdi32",
        "flac",
        "freetype",
        "ogg",
        "openal32",
        "vorbis",
        "vorbisenc",
        "vorbisfile"
    }

    filter "system:windows"
		systemversion "latest"
		defines { "WIN32" }

	filter "configurations:Debug"
		defines { "_DEBUG", "_CONSOLE" }
		symbols "On"
        kind "ConsoleApp"
        links
        {
            "sfml-audio-s-d",
            "sfml-graphics-s-d",
            "sfml-network-s-d",
            "sfml-system-s-d",
            "sfml-window-s-d",
            "imgui-sfml-s-d",
            "yaml-cpp-d"
        }

    filter "configurations:Release"
		defines { "NDEBUG", "NCONSOLE" }
		optimize "On"
        links
        {
            "sfml-audio-s",
            "sfml-graphics-s",
            "sfml-network-s",
            "sfml-system-s",
            "sfml-window-s",
            "imgui-sfml-s",
            "yaml-cpp"
        }

