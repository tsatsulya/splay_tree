cmake_minimum_required(VERSION 3.27)

project(splay_tree)

add_library(splay_tree STATIC)

target_sources(splay_tree
    PRIVATE
        src/splay_tree.cpp
    PUBLIC
        include/splay_tree/splay_tree.h
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

set(graph_dir ${CMAKE_CURRENT_SOURCE_DIR}/..)
target_include_directories(tester PRIVATE ${graph_dir}/preorder_traversal)


set_target_properties(splay_tree PROPERTIES PUBLIC_HEADER "${public_headers}")