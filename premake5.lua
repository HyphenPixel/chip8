---@diagnostic disable: undefined-global, undefined-field
workspace "Chip8"
    configurations { "Debug", "Release" }

project "Chip8"
    kind "ConsoleApp"
    language "C"
    targetdir "output/%{cfg.buildcfg}"

    includedirs { "include" }

    libdirs { os.findlib("raylib") }

    files {
        "**.h",
        "src/**.c"
    }

    links {
        "raylib",
        "GL",
        "m",
        "pthread",
        "dl",
        "rt",
        "X11"
    }

    filter "configurations:Debug"
        defines { "DEBUG" }
        symbols "On"
        warnings "Extra"

    filter "configurations:Release"
        warnings "Off"
        optimize "Full"