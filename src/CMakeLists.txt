add_subdirectory(setup)

list(APPEND EXTRA_LIBS setup)
list(APPEND EXTRA_LIBS stdc++fs)
list(APPEND EXTRA_LIBS git2)

# add the executable
add_executable(uhh uhh.cpp)
set_property(TARGET uhh PROPERTY CXX_STANDARD 17)

target_link_libraries(uhh PUBLIC
    ${EXTRA_LIBS}
)


target_include_directories(uhh PUBLIC
    "${PROJECT_BINARY_DIR}"
)

target_include_directories(uhh PRIVATE
    "${PROJECT_LIB_FOLDER}"
)


