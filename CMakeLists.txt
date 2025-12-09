cmake_minimum_required(VERSION 3.23)
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED True)
# set(VCPKG_TARGET_TRIPLET "x64-linux")
# set(VCPKG_ROOT "$ENV{HOME}/.local/share/vcpkg")
# set(CMAKE_TOOLCHAIN_FILE "${VCPKG_ROOT}/scripts/buildsystems/vcpkg.cmake")
# set(CMAKE_C_COMPILER clang)
# set(CMAKE_CXX_COMPILER clang++)
set(CMAKE_GENERATOR ninja)

# emit compile_flags.json for clangd
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

project(cnake)
add_executable(cnake)

target_sources(cnake PRIVATE
    src/main.cpp
    src/GameState.cpp
    src/GameState.hpp
    src/RenderContext.hpp
    src/RenderContext.cpp
    src/Snake.hpp
    src/Snake.cpp
    src/Common.hpp
    src/Common.cpp
)
find_package(SDL3 CONFIG REQUIRED)
find_package(SDL3_ttf CONFIG REQUIRED)
target_link_libraries(cnake PRIVATE SDL3::SDL3 SDL3_ttf::SDL3_ttf)

target_include_directories(cnake PRIVATE
    ${PROJECT_SOURCE_DIR}/src
)
