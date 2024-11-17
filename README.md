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

# You can choose either GCC or Clang (Default: GCC)
premake cc '<gcc|clang>'

# Generates GNU make files
premake5 gmake2

# Builds the project (Default: release)
make config='<release|debug>'
```
The program will be put in the output directory in either Release or Debug depending on the configuration

### Platform availablity
 - [x] Linux
 - [ ] Windows
 - [ ] Mac
