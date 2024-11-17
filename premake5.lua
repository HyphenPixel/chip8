---@diagnostic disable: undefined-global, undefined-field
newaction {
    trigger = "clean",
    description = "Remove all build files in obj folder",
    execute = function ()
        print("Cleaning build files...")
        os.rmdir("obj")
        print("Clean complete!")
    end
}

newaction {
    trigger = "cc",
    description = "Set the compiler to use (gcc or clang)",
    validargs = { "gcc", "clang" },
    execute = function ()
        local compiler = _ARGS[1]
        if not compiler then
            print("Usage: premake5 cc <gcc|clang>")
            return
        end

        if compiler ~= "gcc" and compiler ~= "clang" then
            print("Invalid compiler. Choose 'gcc' or 'clang'.")
            return
        end

        local config_file = "compiler.cfg"
        local file = io.open(config_file, "w")
        if file then
            file:write(compiler)
            file:close()
            print("Compiler set to: " .. compiler)
        else
            print("Error: Unable to write compiler configuration.")
        end
    end
}

workspace "Chip8"
    configurations { "Debug", "Release" }

project "Chip8"
    kind "ConsoleApp"
    language "C"
    targetdir "output/%{cfg.buildcfg}"

    local config_file = "compiler.cfg"
    local compiler = "gcc" -- Default compiler
    local file = io.open(config_file, "r")
    if file then
        compiler = file:read("*all")
        file:close()
    end

    toolset(compiler)

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