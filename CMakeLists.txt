cmake_minimum_required(VERSION 3.10)

list(APPEND CMAKE_MODULE_PATH ${CMAKE_CURRENT_LIST_DIR}/cmake)

# set the project name
project(uhh VERSION 1.0)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# find_library(LIB_WEB_SOCKETS libwebsockets.so)
# set(PROJECT_LIB_FOLDER ${CMAKE_CURRENT_SOURCE_DIR}/lib)
# configure_file(compile_commands.json ../../TEST_FILE.json COPYONLY)

find_package(libgit2)

add_subdirectory(src)


