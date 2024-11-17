
---@diagnostic disable: undefined-global, undefined-field
workspace "Chip8"
    configurations { "Debug", "Release" }

project "Chip8"
    kind "ConsoleApp"
    language "C"
    targetdir "output/%{cfg.buildcfg}"

    -- Read the compiler setting from a file
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

    filter "configurations:Release"
        defines { "NDEBUG" }
        optimize "On"

newoption {
    trigger = "config",
    value = "Configuration",
    description = "Choose the build configuration (Debug or Release)",
    default = "Debug"
}
        
newoption {
    trigger = "generator",
    value = "Generator",
    description = "Choose the build system generator (e.g., gmake2, vs2022)",
    default = "gmake2"
}

newoption {
    trigger = "rom",
    value = "Rom",
    description = "Choose the rom file to be loaded",
    default = " "
}

-- Action: clean
newaction {
    trigger = "clean",
    description = "Remove all build files in obj folder",
    execute = function ()
        print("Cleaning build files...")
        os.rmdir("obj")
        print("Clean complete!")
    end
}

-- Action: cc
newaction {
    trigger = "cc",
    description = "Set the compiler to use (gcc or clang)",
    validargs = { "gcc", "clang" },
    execute = function ()
        local compiler = _ARGS[1]
        if not compiler then
            print("Usage: premake5 setcompiler <gcc|clang>")
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

-- Action: run
newaction {
    trigger = "run",
    description = "Run the compiled program",
    execute = function ()
        -- Get configuration from options or default to Debug
        local config = _OPTIONS["config"] or "debug"
        local exe_path = string.format("output/%s/Chip8", config)

        -- Check if the executable exists
        if os.isfile(exe_path) then
            print("Running: " .. exe_path)
            local run_status = os.execute(string.format("%s %s", exe_path, _OPTIONS["rom"]))
            if run_status ~= true then
                print("Error: Program exited with an error.")
            end
        else
            print("Error: Executable not found for configuration: " .. config)
            print("Build the project first with 'premake5 build'.")
        end
    end
}

-- Action: build
newaction {
    trigger = "build",
    description = "Generate build files and compile the project",
    execute = function ()
        -- Get generator and configuration options
        local generator = _OPTIONS["generator"] or "gmake2"
        local config = _OPTIONS["config"] or "debug"

        -- Step 1: Generate build files
        print("Generating build files using: " .. generator)
        local generate_cmd = string.format("premake5 %s", generator)
        print("premake5 " .. generator)
        local generate_status = os.execute(generate_cmd)
        if generate_status ~= true then
            print("Error: Failed to generate build files.")
            return
        end

        -- Step 2: Compile the project
        if generator == "gmake" or generator == "gmake2" then
            print("Compiling project with configuration: " .. config)
            local build_cmd = string.format("make config=%s", config)
            local build_status = os.execute(build_cmd)
            if build_status ~= true then
                print("Error: Compilation failed.")
            else
                print("Build completed successfully!")
            end
        else
            print("Build system generated. Open the project in your IDE to build.")
        end
    end
}
