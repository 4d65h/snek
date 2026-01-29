cmake_minimum_required(VERSION 3.16)
project(snek)

# set the output directory for built objects.
# This makes sure that the dynamic library goes into the build directory automatically.
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/$<CONFIGURATION>")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/$<CONFIGURATION>")

option(SNEK_USE_SYSTEM_SDL3 "Use system SDL3" OFF)

# This assumes the SDL source is available in submodules/SDL
message("Building on ${CMAKE_SYSTEM_NAME}")
if(SNEK_USE_SYSTEM_SDL3)
    find_package(SDL3 REQUIRED CONFIG REQUIRED COMPONENTS SDL3-shared)
else()
    add_subdirectory(submodules/SDL EXCLUDE_FROM_ALL)
endif()

# Create your game executable target as usual
add_executable(snek WIN32 snek.c)

# Link to the actual SDL3 library.
target_link_libraries(snek PRIVATE SDL3::SDL3)
