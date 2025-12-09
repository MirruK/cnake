# cnake - Snake implemented for fun and learning

## Goal

The idea with this project was to develop a game without any knowledge
of game development, while simultaneously getting more familiar with the
c++ standard library. I purposefully did not look up best practices for implementing
different aspects of the game logic or architecture. *All uses of AI in this project is
marked with a comment in the form: // AI: XXXX*.

## Decisions and Improvements

Many simplifications were made on purpose to reduce the scope of this project.

- Font rendering and UI drawing is done in a very inflexible way to prevent
re-implementing a UI framework just to make it more clean
- No entity manager is present since the only entities are the snake and the food
- Input handling is done in the most simple way using a switch statement in the event-handler.

### TODOS:

- FPS-Counter: Figure out how to render a non-flickering fps counter
- Release for Windows, MacOS and Linux: Use github actions to build statically linked binaries for the major platforms
- CMake profiles: Create CMake profiles for Debug, Release, NoVCPKG that simplify building the project in differnt cases
- Lint, format, Cleanup: Clean up the code by running linters and formatters in CI
- Improve architecture: Right now singletons are used extensively, but it might not be necessary. Also clean up the public members
of classes to prevent API-pollution.

## Running the game

The game depends on SDL3 and SDL3-ttf.

With the provided CMake-project and vcpkg, the
process is very simple.

### Linux

#### With vcpkg

1. Clone the repository: `git clone https://github.com/Mirruk/cnake.git` 
2. Move into the directory: `cd cnake`
3. Validate that VCPKG_ROOT env-var is set
  - If not: `export VCPKG_ROOT=/path/to/vcpkg`
4. Check that cmake/vcpkg-toolchain.cmake contents are correct, it should work without modification
5. Install dependencies and prepare build directory: `cmake -S . -B build -DCMAKE_TOOLCHAIN_FILE=cmake/vcpkg-toolchain.cmake`
  - If you are weary, packages that get installed are specified in `./vcpkg.json`
6. Compile and link the binary: `cmake --build build`
7. Run the game: `./build/cnake`

#### Without vcpkg

1. Clone the repository: `git clone https://github.com/Mirruk/cnake.git` 
2. Move into the directory: `cd cnake`
3. Install SDL3 and SDL3-ttf: For example with pacman: `sudo pacman -S sdl3 sdl3-ttf`
4. Prepare build directory and locate dependencies: `cmake -S . -B build`
5. Compile and link the binary: `cmake --build build`
6. Run the binary `./build/cnake`

### Windows

#### With vcpkg

1. Clone the repository: `git clone https://github.com/Mirruk/cnake.git` 
2. Move into the directory: `cd cnake`
3. Validate that VCPKG_ROOT env-var is set
  - If not, in powershell run: `$env:VCPKG_ROOT="C:\src\vcpkg`
4. Check that cmake/vcpkg-toolchain.cmake contents are correct, it should work without modification
5. Install dependencies and prepare build directory: `cmake -S . -B build -DCMAKE_TOOLCHAIN_FILE=cmake/vcpkg-toolchain.cmake`
  - If you are weary, packages that get installed are specified in `./vcpkg.json`
6. Compile and link the binary: `cmake --build build`
7. Run the game: `./build/cnake`

#### Without vcpkg

1. Clone the repository: `git clone https://github.com/Mirruk/cnake.git` 
2. Move into the directory: `cd cnake`
3. Install SDL3 and SDL3-ttf:
  - There are various ways to install sdl3 on windows, either with or without Visual Studio.
  - Here is a link to the SDL repo with further instructions: https://github.com/libsdl-org/SDL
4. Prepare build directory and locate dependencies: `cmake -S . -B build`
5. Compile and link the binary: `cmake --build build`
6. Run the binary `./build/cnake`

### MacOS

1. Clone the repository: `git clone https://github.com/Mirruk/cnake.git` 
2. Move into the directory: `cd cnake`
4. Install SDL3 and SDL3-ttf: `brew install sdl3 sdl3-ttf`
5. Prepare build directory and locate dependencies: `cmake -S . -B build`
6. Compile and link the binary: `cmake --build build`
7. Run the binary `./build/cnake`

