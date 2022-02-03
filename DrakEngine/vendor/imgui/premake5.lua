project "ImGui"
    kind "StaticLib"
    language "C++"

    targetdir ("%{wks.location}/bin/" .. outputdir .. "/%{prj.name}")
    objdir ("%{wks.location}/bin-int/" .. outputdir .. "/%{prj.name}")

    files
    {
		"imgui/imconfig.h",
		"imgui/imgui.h",
		"imgui/imgui.cpp",
		"imgui/imgui_draw.cpp",
		"imgui/imgui_internal.h",
		"imgui/imgui_widgets.cpp",
		"imgui/imstb_rectpack.h",
		"imgui/imstb_textedit.h",
		"imgui/imstb_truetype.h",
		"imgui/imgui_demo.cpp",
		"imgui/imgui_tables.cpp"
    }

	filter "system:macosx"
		cppdialect "C++17"
		staticruntime "On"
		files
		{
 		    "imgui/backends/imgui_impl_vulkan.h",
 		    "imgui/backends/imgui_impl_vulkan.cpp",
		    "imgui/backends/imgui_impl_glfw.cpp"
		}
        includedirs
        {
            "%{IncludeDir.ImGui}",
            "/opt/homebrew/include/" -- Needed for Vulkan and GLFW
        }

	filter "configurations:Debug"
		runtime "Debug"
		symbols "on"

	filter "configurations:Release"
		runtime "Release"
		optimize "on"
