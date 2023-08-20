# chip8
A CHIP8 emulator written in C++. This emulator has the functionality to emulate CHIP8 ROMs and also has a disassembler.

## Prerequisites
#### 1. Install SDL2
The display of the emulator is created with the help of the SDL2 framework. To install it, run the following command.
```
# for debian based systems
sudo apt install libsdl2-dev

# for macOS
brew install sdl2
```

#### 2. Configure the correct paths for SDL2 libraries
The include and library paths to your SDL2 Installation can be found using the command:
```
# works on both Linux and macOS

# for include path
sdl2-config --cflags
# for lib path
sdl2-config --libs
```
Change the paths to the SDL2 libraries in the below Makefiles.

For macOS, change the paths inside the if block ```ifeq ($(UNAME), Darwin)```.

For Linux, change the paths inside the if block ```ifeq ($(UNAME), Linux)```.

In ```/path/to/chip8/gpu/src/Makefile```, change the include path in the variable ```INC_FLAGS```
and the lib path in the variable ```LIBS```

In ```/path/to/chip8/main/src/Makefile```, change the include path in the variable ```INC_FLAGS```
and the lib path in the variable ```LIB_FLAGS```

#### 3. (FOR macOS ONLY) Install GCC
Install gcc from brew
```
brew install gcc
```
The clang compiler included in macOS wasn't compiling my code at all, so I switched to GCC.

## Compilation
Navigate to
```
/path/to/chip8/main/src/
```
Then run the following commands IN ORDER to compile the modules and create the final executable binary.
```
# The following command compiles the modules, cpu and gpu
make dependency
# The following command creates the final executables
make
```
This creates executables in the folder ```/path/to/chip8/src/bin/```. The directory also contains some test
ROMs inside the ```roms``` director.

## Execution
### Emulator
After building, the final executables will be found in the directory
```
/path/to/chip8/src/bin/
```
The emulator can be started with the ```emulator``` binary.
```
./emulator <Scale> <Delay> <Path to ROM>
```
The disassembler can be started from the ```disassembler``` binary.
```
./disassembler <Path to ROM>
```
