cmake_minimum_required(VERSION 3.10)

# set the project name
project(uhh VERSION 1.0)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# find_library(LIB_WEB_SOCKETS libwebsockets.so)
# set(PROJECT_LIB_FOLDER ${CMAKE_CURRENT_SOURCE_DIR}/lib)

add_subdirectory(src)

