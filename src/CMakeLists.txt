add_subdirectory(uhh)
add_subdirectory(commands)
add_subdirectory(git)
add_subdirectory(config)
add_subdirectory(term)

list(APPEND EXTRA_LIBS uhh-class)
list(APPEND EXTRA_LIBS commands)
list(APPEND EXTRA_LIBS uhh-git)
list(APPEND EXTRA_LIBS config-class)
list(APPEND EXTRA_LIBS stdc++fs)
list(APPEND EXTRA_LIBS git2)

add_executable(uhh main.cpp)

# add the executable
set_property(TARGET uhh PROPERTY CXX_STANDARD 17)
set(EXECUTABLE_OUTPUT_PATH  ${PROJECT_SOURCE_DIR}/build/)

target_link_libraries(uhh PUBLIC
    ${EXTRA_LIBS}
)

target_include_directories(uhh PUBLIC
    "${PROJECT_BINARY_DIR}"
)

target_include_directories(uhh PRIVATE
    "${PROJECT_LIB_FOLDER}"
)
