cmake_minimum_required(VERSION 3.10)

project(
        preorder_traversal
        LANGUAGES CXX
        )

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)
set(CMAKE_CXX_EXTENSIONS False)

option(DEBUG_FLAGS "Use debug compilation flags" OFF)
if ("${CMAKE_BUILD_TYPE}" STREQUAL "Release")
        set(DEBUG_FLAGS OFF)
endif()

if (DEBUG_FLAGS)
        set(
        library_compile_options
        -O0 -g -Wpedantic -Wall -Wextra -Wcast-align -Wcast-qual
        -Wctor-dtor-privacy -Wdisabled-optimization -Wformat=2
        -Winit-self -Wmissing-declarations -Wmissing-include-dirs
        -Wold-style-cast -Woverloaded-virtual -Wredundant-decls
        -Wsign-conversion -Wsign-promo -Wstrict-overflow=5
        -Wswitch-default -Wundef -Wunused -Wshadow
        )
else()
        set(library_compile_options -O2)
endif()

add_library(preorder_traversal INTERFACE)
target_include_directories(preorder_traversal INTERFACE ${CMAKE_CURRENT_SOURCE_DIR})

target_compile_features(preorder_traversal INTERFACE cxx_std_17)

target_compile_options(
        preorder_traversal INTERFACE
        $<BUILD_INTERFACE:${library_compile_options}>
        )