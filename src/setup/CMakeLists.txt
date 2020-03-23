add_library(setup setup.cpp setup.h)

target_include_directories(setup INTERFACE
    ${CMAKE_CURRENT_SOURCE_DIR}
)
target_include_directories(setup PRIVATE
    ${PROJECT_LIB_FOLDER}
)

