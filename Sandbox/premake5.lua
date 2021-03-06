project "Sandbox"
	kind "ConsoleApp"
	language "C++"
	cppdialect "C++17"

	targetdir ("%{wks.location}/bin/" .. outputdir .. "/%{prj.name}")
	objdir ("%{wks.location}/bin-int/" .. outputdir .. "/%{prj.name}")

	files
	{
		"src/**.h",
		"src/**.cpp"
	}

	includedirs
	{
        "%{wks.location}/DrakEngine/vendor/imgui/imgui",
        "%{wks.location}/DrakEngine/vendor/spdlog/include",
        "%{wks.location}/DrakEngine/src",
        "src"
	}

	libdirs
	{
	    "/opt/homebrew/lib"
	}

    links
    {
        "glfw",
        "DrakEngine",
        "ImGui",
        "Vulkan"
    }

	filter "system:macosx"
		cppdialect "C++17"
		staticruntime "On"
        libdirs
        {
            "/usr/local/lib" -- Needed for -lVulkan
    	}
        includedirs { "/opt/homebrew/include/" } -- Needed for GLFW brew installed headers

	filter "configurations:Debug"
        defines "DRAK_DEBUG"
        runtime "Debug"
        symbols "on"


    filter "configurations:Release"
        defines "DRAK_RELEASE"
        runtime "Release"
        optimize "on"
