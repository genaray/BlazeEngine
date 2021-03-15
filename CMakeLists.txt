cmake_minimum_required(VERSION 3.17)
project(Engine)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_VERBOSE_MAKEFILE ON)
include_directories(include)
include_directories(src/headers)

# Set this to the minimal version you want to support
find_package(raylib 3.0 QUIET) # Let CMake search for a raylib-config.cmake

# You could change the QUIET above to REQUIRED and remove this if() clause
# This part downloads raylib and builds it if it's not installed on your system
if (NOT raylib_FOUND) # If there's none, fetch and build raylib
    include(FetchContent)

    FetchContent_Declare(
            raylib
            URL https://github.com/raysan5/raylib/archive/master.tar.gz
    )

    FetchContent_GetProperties(raylib)
    if (NOT raylib_POPULATED) # Have we downloaded raylib yet?
        set(FETCHCONTENT_QUIET NO)
        FetchContent_Populate(raylib)

        set(BUILD_EXAMPLES OFF CACHE BOOL "" FORCE) # don't build the supplied examples

        # build raylib
        add_subdirectory(${raylib_SOURCE_DIR} ${raylib_BINARY_DIR})

    endif()

endif()

SET(CMAKE_CXX_FLAGS  "${CMAKE_CXX_FLAGS}")
# This is the main part:
add_executable(${PROJECT_NAME} src/main.cpp src/ColorUtils.cpp src/headers/ColorUtils.h src/VectorUtils.cpp src/headers/VectorUtils.h src/headers/EnvironmentUtils.h src/EnvironmentUtils.cpp src/ModelUtils.cpp src/headers/ModelUtils.h)
target_link_libraries(${PROJECT_NAME} raylib)