# Another Chip8 Interpreter
This is just a simple chip8 interpreter that is written in pure C using Raylib for the rendering and sound.

### Building Chip8
A few prerequisites:
1. Raylib and any of it's required libs
    - Getting Raylib setup will get you everything you need for this project as well
2. Premake5
3. GCC or Clang

Build Instructions:
```bash
git clone https://github.com/HyphenPixel/chip8.git
cd chip8

# If you want to use a defferent compiler you can choose either GCC or Clang (Default: GCC)
premake5 cc '<gcc|clang>'

# Generates make files with a specified configuration and generator (i.e. gmake2, vs2022, etc...)
# The program will be put in the 'output' directory in either the 'release' or 'debug' sub-directory depending on the configuration
# (Defaults: config=Debug, generator=gmake2)
premake5 build --config='<release|debug>' --generator='<generator>'

# Runs the interpreter with the specified rom file
# (Defaults: config=Debug, rom=None)
premake5 run --config='<release|debug>' --rom='<path/to/rom_file>'
```
To run the interpreter you can ues either command:
1. `premake5 run --config='<release|debug>' --rom='<path/to/rom_file>'`
2. `./output/<configuration>/Chip8 <path/to/rom_file>`

The main executable doesn't *need* to be in any particular folder so you can move it to somewhere more convenient

If you want to code your own roms or download some premade test roms you can go here [https://chipo.ber.gp/](https://chipo.ber.gp/)

### Platform availablity
 - [x] Linux
 - [ ] Windows
 - [ ] Mac
