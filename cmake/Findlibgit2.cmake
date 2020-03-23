cmake_minimum_required(VERSION 3.10)

set(GIT2_ROOT_DIR "${CMAKE_CURRENT_LIST_DIR}/../3rdparty/libgit2")

add_subdirectory(${GIT2_ROOT_DIR} ${CMAKE_BINARY_DIR}/3rdparty/libgit2)

add_library(git2::git2 ALIAS git2)
target_include_directories(git2 PUBLIC
	$<BUILD_INTERFACE:${GIT2_ROOT_DIR}/include>
)
