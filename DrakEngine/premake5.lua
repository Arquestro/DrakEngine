project "DrakEngine"
    kind "StaticLib"
    language "C++"
    cppdialect "C++17"
    staticruntime "On"
    
	targetdir ("%{wks.location}/bin/" .. outputdir .. "/%{prj.name}")
	objdir ("%{wks.location}/bin-int/" .. outputdir .. "/%{prj.name}")

    pchheader ("%{prj.name}/src/dkpch.h")
    pchsource ("%{prj.name}/src/dkpch.cpp")

    files
    {
        "src/**.h",
        "src/**.cpp"
    }

    includedirs
    {
        "src",
        "vendor/spdlog/include",
        "%{IncludeDir.ImGui}"
    }

    links
    {
        "ImGui"
    }

    filter "system:macosx"
        defines
        {
            "DRAK_PLATFORM_MACOS"
		}
        includedirs { "/opt/homebrew/include/" } -- Needed for GLFW brew installed headers

	filter "configurations:Debug"
        defines "DRAK_DEBUG"
        runtime "Debug"
        symbols "on"
        links {}

    filter "configurations:Release"
        defines "DRAK_RELEASE"
        runtime "Release"
        optimize "on"
        links {}

