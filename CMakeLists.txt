cmake_minimum_required(VERSION 3.27)

project(splay_tree)

add_library(splay_tree STATIC)

# no need to add headers here, only sources are required
target_sources(splay_tree
    PRIVATE
        src/splay_tree.cpp
)

target_link_libraries(splay_tree INTERFACE preorder_traversal)

target_include_directories(splay_tree
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/src
    PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>
)

set(public_headers
    include/*.h
)
# note that ${public_headers} has to be in quotes

set(graph_dir ${CMAKE_CURRENT_SOURCE_DIR}/..)
target_include_directories(splay_tree PRIVATE ${graph_dir}/preorder_traversal)

set_target_properties(splay_tree PROPERTIES PUBLIC_HEADER "${public_headers}")