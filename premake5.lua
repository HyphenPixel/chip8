---@diagnostic disable: undefined-global, undefined-field
workspace "Chip8"
    configurations {"Debug", "Release"}

    includedirs {"include"}

    filter "configurations:Debug"
        defines {"DEBUG"}
        symbols "On"
        targetdir "output/debug"
    
    filter "configurations:Release"
        flags {"NoWarnings"}
        optimize "Full"
        targetdir "output/release"

project "Chip8"
    kind "ConsoleApp"
    language "C"
    location "build"
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
    libdirs {os.findlib("raylib")}