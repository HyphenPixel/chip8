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
        "-lraylib", 
        "-lGL", 
        "-lm", 
        "-lpthread", 
        "-ldl", 
        "-lrt", 
        "-lX11"
    }
    libdirs {os.findlib("raylib")}