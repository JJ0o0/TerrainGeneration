cmake_minimum_required(VERSION 3.20)
project(TerrainGenerationSDL3Project)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

find_package(SDL3 REQUIRED)
find_package(SDL3_ttf REQUIRED)
find_package(OpenGL REQUIRED)
find_package(spdlog REQUIRED)

# Source - https://stackoverflow.com/a/65133324
# Posted by tdashroy, modified by community. See post 'Timeline' for change history
# Retrieved 2026-05-16, License - CC BY-SA 4.0

add_custom_target(copy_assets
    COMMAND ${CMAKE_COMMAND} -P ${CMAKE_CURRENT_LIST_DIR}/copy_assets.cmake
)

add_executable(app 
src/main.cpp
src/core/App.cpp
src/graphics/Mesh.cpp
src/graphics/Shader.cpp
src/graphics/Terrain.cpp
src/graphics/Camera.cpp
src/utilities/Log.cpp
src/utilities/UI.cpp
vendor/glad.c
vendor/stb_perlin.cpp
)

add_dependencies(app copy_assets)

target_include_directories(app PRIVATE include vendor)
target_link_libraries(app PRIVATE SDL3::SDL3 SDL3_ttf::SDL3_ttf OpenGL::GL spdlog::spdlog)