﻿cmake_minimum_required(VERSION 3.10)

if(NOT DEFINED CMAKE_TOOLCHAIN_FILE)
    set(CMAKE_TOOLCHAIN_FILE
        "${CMAKE_CURRENT_SOURCE_DIR}/vcpkg/scripts/buildsystems/vcpkg.cmake"
        CACHE STRING "Vcpkg toolchain file")
endif()

project(
    rogue  # Project name, change this as needed.
    LANGUAGES C CXX
)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${PROJECT_BINARY_DIR}/bin")

file(
    GLOB_RECURSE SOURCE_FILES
    CONFIGURE_DEPENDS  # Automatically reconfigure if source files are added/removed.
    ${PROJECT_SOURCE_DIR}/src/*.cpp
    ${PROJECT_SOURCE_DIR}/src/*.hpp
)
add_executable(${PROJECT_NAME} ${SOURCE_FILES})

# Ensure the C++20 standard is available.
target_compile_features(${PROJECT_NAME} PRIVATE cxx_std_20)

# Enforce UTF-8 encoding on MSVC.
if (MSVC)
    target_compile_options(${PROJECT_NAME} PRIVATE /utf-8)
endif()

# Enable warnings recommended for new projects.
if (MSVC)
    target_compile_options(${PROJECT_NAME} PRIVATE /W4)
else()
    target_compile_options(${PROJECT_NAME} PRIVATE -Wall -Wextra)
endif()

find_package(libtcod CONFIG REQUIRED)
find_package(fmt CONFIG REQUIRED)
find_package(SDL2 CONFIG REQUIRED)

add_subdirectory(external/flecs)

target_link_libraries(
    ${PROJECT_NAME}
    PRIVATE
        SDL2::SDL2
        SDL2::SDL2main
        libtcod::libtcod
        fmt::fmt
        flecs::flecs_static
)

# Copy the data directory to the build directory
add_custom_command(
    TARGET ${PROJECT_NAME} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
    ${CMAKE_SOURCE_DIR}/data $<TARGET_FILE_DIR:${PROJECT_NAME}>/data
)

# Ensure the data directory exists at the source location
if(NOT EXISTS ${CMAKE_SOURCE_DIR}/data)
    message(FATAL_ERROR "The data directory does not exist in the source directory.")
endif()
