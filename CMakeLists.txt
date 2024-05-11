
cmake_minimum_required(VERSION 3.10)

set(graphs_dir ${CMAKE_CURRENT_SOURCE_DIR}/..)
include_directories(${graphs_dir}/preorder_traversal)

file(GLOB_RECURSE SRC_FILES src/*.cpp)
add_executable(splay_tree ${SRC_FILES})
set_target_properties(
	splay_tree PROPERTIES
	RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin
)
# target_link_libraries(splay_tree PRIVATE cachelet)
target_include_directories(splay_tree PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)

# configure_file(${CNF}/auto_conf.hpp.in auto_conf.hpp @ONLY)
target_include_directories(splay_tree PRIVATE ${CMAKE_CURRENT_BINARY_DIR})