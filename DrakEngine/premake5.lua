project "DrakEngine"
    kind "StaticLib"
    language "C++"
    cppdialect "C++17"
    staticruntime "On"
    
	targetdir ("%{wks.location}/bin/" .. outputdir .. "/%{prj.name}")
	objdir ("%{wks.location}/bin-int/" .. outputdir .. "/%{prj.name}")

    files
    {
        "src/**.h",
        "src/**.cpp"
    }

    includedirs
    {
        "src"
    }

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

