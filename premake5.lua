---@diagnostic disable: undefined-global, undefined-field
workspace "Chip8"
    configurations {"Debug", "Release"}
    location "build"

    includedirs {"include"}

    filter "configurations:Debug"
        defines {"DEBUG"}
        symbols "On"
        warnings "Everything"
        targetdir "output/debug"
    
    filter "configurations:Release"
        warnings "Off"
        optimize "Full"
        targetdir "output/release"

project "Chip8"
    kind "ConsoleApp"
    language "C"
    location "build"
    libdirs {os.findlib("raylib")}
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