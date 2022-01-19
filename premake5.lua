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
group ""

include "DrakEngine"
include "Sandbox"
