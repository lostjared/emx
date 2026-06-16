cmake_minimum_required(VERSION 3.10)

project(emx LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})

add_compile_options(-O2 -Wall -pedantic)

set(EMX_SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/src)
set(SOURCES
    ${EMX_SOURCE_DIR}/camera.cpp
    ${EMX_SOURCE_DIR}/game.cpp
    ${EMX_SOURCE_DIR}/game_level.cpp
    ${EMX_SOURCE_DIR}/gfx_data.cpp
    ${EMX_SOURCE_DIR}/gfx_file.cpp
    ${EMX_SOURCE_DIR}/level.cpp
    ${EMX_SOURCE_DIR}/object.cpp
    ${EMX_SOURCE_DIR}/particles.cpp
    ${EMX_SOURCE_DIR}/star_field.cpp
    ${EMX_SOURCE_DIR}/window.cpp
)

set(HEADERS
    ${EMX_SOURCE_DIR}/camera.hpp
    ${EMX_SOURCE_DIR}/game.hpp
    ${EMX_SOURCE_DIR}/game_level.hpp
    ${EMX_SOURCE_DIR}/gfx_data.hpp
    ${EMX_SOURCE_DIR}/gfx_file.hpp
    ${EMX_SOURCE_DIR}/level.hpp
    ${EMX_SOURCE_DIR}/object.hpp
    ${EMX_SOURCE_DIR}/particles.hpp
    ${EMX_SOURCE_DIR}/star_field.hpp
    ${EMX_SOURCE_DIR}/version.hpp
    ${EMX_SOURCE_DIR}/window_const.hpp
    ${EMX_SOURCE_DIR}/window.hpp
    ${EMX_SOURCE_DIR}/argz.hpp
)

find_package(SDL2 REQUIRED)
find_package(SDL2_ttf REQUIRED)

add_library(emxStatic STATIC ${SOURCES})
set_target_properties(emxStatic PROPERTIES OUTPUT_NAME "emx")
target_include_directories(emxStatic PUBLIC ${EMX_SOURCE_DIR})
target_link_libraries(emxStatic PRIVATE ${SDL2_LIBRARIES} SDL2_ttf::SDL2_ttf)

add_library(emxShared SHARED ${SOURCES})
set_target_properties(emxShared PROPERTIES OUTPUT_NAME "emx")
target_include_directories(emxShared PUBLIC ${EMX_SOURCE_DIR})
target_link_libraries(emxShared PRIVATE ${SDL2_LIBRARIES} SDL2_ttf::SDL2_ttf)

install(FILES ${HEADERS} DESTINATION include/emx)
install(TARGETS emxStatic emxShared
    ARCHIVE DESTINATION lib
    LIBRARY DESTINATION lib
)

include(CMakePackageConfigHelpers)
configure_package_config_file(
    "${CMAKE_CURRENT_SOURCE_DIR}/emxConfig.cmake.in"
    "${CMAKE_CURRENT_BINARY_DIR}/emxConfig.cmake"
    INSTALL_DESTINATION lib/cmake/emx
)
install(FILES "${CMAKE_CURRENT_BINARY_DIR}/emxConfig.cmake"
    DESTINATION lib/cmake/emx
)

configure_file(
    "${CMAKE_CURRENT_SOURCE_DIR}/cmake_uninstall.cmake.in"
    "${CMAKE_CURRENT_BINARY_DIR}/cmake_uninstall.cmake"
    @ONLY
)

add_custom_target(uninstallddd
    COMMAND ${CMAKE_COMMAND} -P ${CMAKE_CURRENT_BINARY_DIR}/cmake_uninstall.cmake
)

set(SCROLLER_SOURCE ${CMAKE_CURRENT_SOURCE_DIR}/scroller/main.cpp)

add_executable(gfx-cmp ${CMAKE_CURRENT_SOURCE_DIR}/gfx/gfx_main.cpp)
target_link_libraries(gfx-cmp PRIVATE ${SDL2_LIBRARIES} emxShared)
install(TARGETS gfx-cmp RUNTIME DESTINATION bin)

add_executable(lvl-convert ${CMAKE_CURRENT_SOURCE_DIR}/convert/main.cpp)
target_link_libraries(lvl-convert PRIVATE ${SDL2_LIBRARIES} emxShared)
install(TARGETS lvl-convert RUNTIME DESTINATION bin)

add_executable(scroller ${SCROLLER_SOURCE})
target_include_directories(scroller PRIVATE ${EMX_SOURCE_DIR})
target_link_libraries(scroller PRIVATE ${SDL2_LIBRARIES} emxShared)

set(SCROLLER_ASSET_SOURCE ${CMAKE_CURRENT_SOURCE_DIR}/scroller/assets)
set(SCROLLER_ASSET_DEST ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/assets)
add_custom_target(scroller_assets ALL
    COMMAND ${CMAKE_COMMAND} -E copy_directory
            ${SCROLLER_ASSET_SOURCE}
            ${SCROLLER_ASSET_DEST}
    COMMENT "Copying scroller assets to the build directory"
)
add_dependencies(scroller scroller_assets)

add_subdirectory(editor)

if(TARGET editor)
    set_target_properties(editor PROPERTIES
        RUNTIME_OUTPUT_DIRECTORY ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}
        LIBRARY_OUTPUT_DIRECTORY ${CMAKE_LIBRARY_OUTPUT_DIRECTORY}
        ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_ARCHIVE_OUTPUT_DIRECTORY}
    )
endif()
