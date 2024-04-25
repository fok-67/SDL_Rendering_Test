cmake_minimum_required(VERSION 3.5)

project(SDL_Test)

set(CMAKE_CXX_STANDARD 20)

if(CMAKE_SYSTEM_NAME STREQUAL "Linux")
    # set toolchain
    set(tools /usr/local/gcc-13.2.0)
    set(CMAKE_C_COMPILER ${tools}/bin/gcc)
    set(CMAKE_CXX_COMPILER ${tools}/bin/g++)
endif()

# Create an option to switch between a system sdl library and a vendored sdl library
option(MYGAME_VENDORED "Use vendored libraries" OFF)

if(MYGAME_VENDORED)
    add_subdirectory(vendored/sdl EXCLUDE_FROM_ALL)
else()
    # 1. Look for a SDL2 package, 2. look for the SDL2 component and 3. fail if none can be found
    find_package(SDL2 REQUIRED CONFIG REQUIRED COMPONENTS SDL2)

    # 1. Look for a SDL2 package, 2. Look for the SDL2maincomponent and 3. DO NOT fail when SDL2main is not available
    find_package(SDL2 REQUIRED CONFIG COMPONENTS SDL2main)
endif()

# Create your executable target as usual
add_executable(SDL_test main.cpp Renderer.cpp)

# SDL2::SDL2main may or may not be available. It is e.g. required by Windows GUI applications
if(TARGET SDL2::SDL2main)
    # It has an implicit dependency on SDL2 functions, so it MUST be added before SDL2::SDL2 (or SDL2::SDL2-static)
    target_link_libraries(SDL_test PRIVATE SDL2::SDL2main)
endif()

# Link to the actual SDL2 library. SDL2::SDL2 is the shared SDL library, SDL2::SDL2-static is the static SDL library.
target_link_libraries(SDL_test PRIVATE SDL2::SDL2)
