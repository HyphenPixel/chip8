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
premake5 gmake2

# Change 'release' to 'debug' if you want to build with debugging
make config=release
```
The program will be put in the ouput directory in either Release or Debug depending on the configuration

### Platform availablity
 - [x] Linux
 - [ ] Windows
 - [ ] Mac
 - [ ] Raspberry pi 
