# AI: Thanks gipiti for making this work on all platforms

# Require the user to provide their VCPKG_ROOT.
if (NOT DEFINED ENV{VCPKG_ROOT})
    message(FATAL_ERROR "VCPKG_ROOT environment variable must be set.")
endif()

set(VCPKG_ROOT "$ENV{VCPKG_ROOT}" CACHE PATH "")
set(CMAKE_TOOLCHAIN_FILE "${VCPKG_ROOT}/scripts/buildsystems/vcpkg.cmake"
    CACHE FILEPATH "")

# Optional: set your preferred compiler
if (NOT CMAKE_C_COMPILER)
    set(CMAKE_C_COMPILER clang CACHE STRING "")
endif()

if (NOT CMAKE_CXX_COMPILER)
    set(CMAKE_CXX_COMPILER clang++ CACHE STRING "")
endif()

