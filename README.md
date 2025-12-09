# cnake - Snake implemented for fun and learning

## Goal

The idea with this project was to develop a game without any knowledge
of game development, while simultaneously getting more familiar with the
c++ standard library. I purposefully did not look up best practices for implementing
different aspects of the game logic or architecture. *All uses of AI in this project is
marked with a comment in the form: // AI: XXXX*.


## Running the game

The game depends on SDL3 and SDL3-ttf.

With the provided CMake-project and vcpkg, the
process is fully automatic.

1. Clone the repository: `git clone https://github.com/Mirruk/cnake.git` 
2. Move into the directory: `cd cnake`
3. *(IF YOU USE VCPKG):* Install dependencies and prepare build directory: `cmake -S . -B build`
  - If you are weary, packages that get installed are specified in `./vcpkg.json`
  - Move on to step 6.
4. *(IF YOU DON'T USE VCPKG):* Install SDL3 and SDL3-ttf
5. Prepare build directory and locate dependencies: `cmake -B build`
6. Compile and link the binary: `cmake --build build`
7. Run the binary `./build/cnake`
