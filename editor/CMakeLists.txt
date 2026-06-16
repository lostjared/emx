cmake_minimum_required(VERSION 3.16)

project(editor LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

find_package(Qt6 QUIET COMPONENTS Widgets)
if(Qt6_FOUND)
    set(QT_PACKAGE Qt6)
else()
    find_package(Qt5 REQUIRED COMPONENTS Widgets)
    set(QT_PACKAGE Qt5)
endif()

add_executable(editor
    main.cpp
    main_window.cpp
    new_window.cpp
    tool_window.cpp
    run_window.cpp
    debug_window.cpp
    gfx_window.cpp
    open_window.cpp
    about_window.cpp
    pref_window.cpp
    export_window.cpp
    resources.qrc
    main_window.hpp
    new_window.hpp
    tool_window.hpp
    run_window.hpp
    debug_window.hpp
    gfx_window.hpp
    open_window.hpp
    about_window.hpp
    pref_window.hpp
    export_window.hpp
    version_info.hpp
)

set(_editor_output_dir ${CMAKE_RUNTIME_OUTPUT_DIRECTORY})
if(NOT _editor_output_dir)
    set(_editor_output_dir ${CMAKE_CURRENT_BINARY_DIR})
endif()

set_target_properties(editor PROPERTIES
    OUTPUT_NAME emx-editor
    RUNTIME_OUTPUT_DIRECTORY ${_editor_output_dir}
    LIBRARY_OUTPUT_DIRECTORY ${CMAKE_LIBRARY_OUTPUT_DIRECTORY}
    ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_ARCHIVE_OUTPUT_DIRECTORY}
)

target_include_directories(editor PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_CURRENT_LIST_DIR}/../src
)

if(TARGET emxShared)
    target_link_libraries(editor PRIVATE ${QT_PACKAGE}::Widgets emxShared)
else()
    find_library(EMX_LIBRARY
        NAMES emx
        PATHS
            ${CMAKE_CURRENT_LIST_DIR}/../build
            /usr/local/lib
        REQUIRED
    )
    target_link_libraries(editor PRIVATE ${QT_PACKAGE}::Widgets ${EMX_LIBRARY})
endif()
