# Another Chip8 Interpreter
I've been wanting to create an emulator for a while, with the goal of learning about emulation, among other things, and I've finally gotten around to doing it. I hope to revisit this project eventually to improve it or perhaps start over, applying my newfound knowledge.

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

# If you want to use a different compiler you can choose either GCC or Clang 
# (Default: GCC)
premake5 cc '<gcc|clang>'

# Builds the program using a specified generator 
# (Defaults: config=debug, generator=gmake2)
premake5 build --config='<release|debug>' --generator='<generator>'

# Runs the interpreter with the specified rom file
# (Defaults: config=debug, rom=None)
premake5 run --config='<release|debug>' --rom='<path/to/rom_file>'
```
To run the interpreter you can use either command:
1. `premake5 run --config='<release|debug>' --rom='<path/to/rom_file>'`
2. `./output/<release|debug>/Chip8 <path/to/rom_file>`

The main executable doesn't *need* to be in any particular folder so you can move it to somewhere more convenient

If you want to code your own roms or download some premade test roms you can go here [https://chipo.ber.gp/](https://chipo.ber.gp/)

### Platform availablity
 - [x] Linux
 - [ ] Windows
 - [ ] Mac
