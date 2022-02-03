include "Dependencies.lua"

workspace "DrakEngine"
    architecture "ARM64"
    startproject "Sandbox"

    configurations
    {
        "Debug",
        "Release"
    }

outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

group "Dependencies"
    include "vendor/premake"
    include "DrakEngine/vendor/imgui"
group ""

include "DrakEngine"
include "Sandbox"
