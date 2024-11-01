cmake_minimum_required(VERSION 3.10)

# Project name
project(emx)

# Require C++20
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Add compiler flags for optimization and warnings
add_compile_options(-O2 -Wall -pedantic)

# Source files and headers in the src/ subdirectory with full paths
set(SOURCES
    ${CMAKE_CURRENT_SOURCE_DIR}/src/camera.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/game.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/game_level.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/gfx_data.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/gfx_file.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/level.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/object.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/particles.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/star_field.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/window.cpp
)

# Header files in the src/ directory with full paths (using .hpp)
set(HEADERS
    ${CMAKE_CURRENT_SOURCE_DIR}/src/camera.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/game.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/game_level.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/gfx_data.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/gfx_file.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/level.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/object.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/particles.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/star_field.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/version.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/window_const.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/window.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/argz.hpp
)

# Find SDL2 and SDL2_ttf packages
find_package(SDL2 REQUIRED)
find_package(SDL2_ttf REQUIRED)
# Find SDL2_ttf library and include paths
#find_library(SDL2_TTF_LIBRARY NAMES SDL2_ttf PATHS /usr/lib /usr/local/lib)
#find_path(SDL2_TTF_INCLUDE_DIR SDL_ttf.h PATHS /usr/include/SDL2 /usr/local/include/SDL2)

# Set SDL2_TTF_INCLUDE_DIRS if found
#if(SDL2_TTF_INCLUDE_DIR)
#    set(SDL2_TTF_INCLUDE_DIRS ${SDL2_TTF_INCLUDE_DIR})
#    message(STATUS "SDL2_TTF_INCLUDE_DIRS = ${SDL2_TTF_INCLUDE_DIRS}")
#else()
#    message(FATAL_ERROR "SDL2_ttf headers not found!")
#endif()
# Print the found library and include paths to verify
#message(STATUS "SDL2_TTF_LIBRARY = ${SDL2_TTF_LIBRARY}")
message(STATUS "SDL2_TTF_INCLUDE_DIRS = ${SDL2_TTF_INCLUDE_DIRS}")

# Include SDL2 and SDL2_ttf headers
include_directories(${SDL2_INCLUDE_DIRS} ${SDL2_TTF_INCLUDE_DIRS} ${CMAKE_CURRENT_SOURCE_DIR}/src)

# Create static library and name it libemx.a
add_library(emxStatic STATIC ${SOURCES})
set_target_properties(emxStatic PROPERTIES OUTPUT_NAME "emx")

# Create shared library and name it libemx.so
add_library(emxShared SHARED ${SOURCES})
set_target_properties(emxShared PROPERTIES OUTPUT_NAME "emx")

# Link SDL2 and SDL2_ttf with the emx libraries
target_link_libraries(emxStatic ${SDL2_LIBRARIES} SDL2_ttf::SDL2_ttf)
target_link_libraries(emxShared ${SDL2_LIBRARIES} SDL2_ttf::SDL2_ttf)

# Install header files to include/emx (using .hpp files)
install(FILES ${HEADERS} DESTINATION include/emx)

# Install libraries to lib directory
install(TARGETS emxStatic emxShared
    ARCHIVE DESTINATION lib  # For static library (.a)
    LIBRARY DESTINATION lib  # For shared library (.so)
)

# Generate the emxConfig.cmake file for other projects
include(CMakePackageConfigHelpers)

# Configure and generate the config file for the library
configure_package_config_file(
    "${CMAKE_CURRENT_SOURCE_DIR}/emxConfig.cmake.in"
    "${CMAKE_CURRENT_BINARY_DIR}/emxConfig.cmake"
    INSTALL_DESTINATION lib/cmake/emx
)

# Install the config file
install(FILES "${CMAKE_CURRENT_BINARY_DIR}/emxConfig.cmake"
    DESTINATION lib/cmake/emx
)

# Add uninstall target
configure_file(
    "${CMAKE_CURRENT_SOURCE_DIR}/cmake_uninstall.cmake.in"
    "${CMAKE_CURRENT_BINARY_DIR}/cmake_uninstall.cmake"
    @ONLY
)

add_custom_target(uninstallddd
    COMMAND ${CMAKE_COMMAND} -P ${CMAKE_CURRENT_BINARY_DIR}/cmake_uninstall.cmake
)

# ----- Second target for the scroller executable -----

# Source file for scroller
set(SCROLLER_SOURCE ${CMAKE_CURRENT_SOURCE_DIR}/scroller/main.cpp)

add_executable(gfx-cmp ${CMAKE_CURRENT_SOURCE_DIR}/gfx/gfx_main.cpp)
target_link_libraries(gfx-cmp emxStatic)
install(TARGETS gfx-cmp RUNTIME DESTINATION bin)
add_executable(lvl-convert ${CMAKE_CURRENT_SOURCE_DIR}/convert/main.cpp)
target_link_libraries(lvl-convert emxStatic)
install(TARGETS lvl-convert RUNTIME DESTINATION bin)  
# Add executable target for scroller
add_executable(scroller ${SCROLLER_SOURCE})

# Link scroller to emxShared, SDL2, and SDL2_ttf
target_link_libraries(scroller emxShared ${SDL2_LIBRARIES} SDL2_ttf::SDL2_ttf)

# Specify include directories for scroller (for SDL2, SDL2_ttf, and emx headers)
target_include_directories(scroller PRIVATE ${SDL2_INCLUDE_DIRS} ${SDL2_TTF_INCLUDE_DIRS} ${CMAKE_CURRENT_SOURCE_DIR}/src)