cmake_minimum_required(VERSION 3.16)
project(snek)

# set the output directory for built objects.
# This makes sure that the dynamic library goes into the build directory automatically.
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/$<CONFIGURATION>")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/$<CONFIGURATION>")

# This assumes the SDL source is available in submodules/SDL
message("${CMAKE_SYSTEM_NAME}")
if (CMAKE_SYSTEM_NAME STREQUAL "NintendoSwitch")
    message("Building with SDL3-Switch")
    add_subdirectory(submodules/SDL3-NX EXCLUDE_FROM_ALL)
else()
    message("Building with SDL3")
    add_subdirectory(submodules/SDL EXCLUDE_FROM_ALL)
endif ()

# Create your game executable target as usual
add_executable(snek WIN32 snek.c)

# Link to the actual SDL3 library.
target_link_libraries(snek PRIVATE SDL3::SDL3)