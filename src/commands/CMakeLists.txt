add_library(commands commands.cpp commands.h)

target_include_directories(commands INTERFACE
    ${CMAKE_CURRENT_SOURCE_DIR}
)

target_include_directories(commands PRIVATE
    ${PROJECT_LIB_FOLDER}
)

