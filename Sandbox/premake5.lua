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
        "DrakEngine"
    }

	filter "configurations:Debug"
        defines "DRAK_DEBUG"
        runtime "Debug"
        symbols "on"


    filter "configurations:Release"
        defines "DRAK_RELEASE"
        runtime "Release"
        optimize "on"
