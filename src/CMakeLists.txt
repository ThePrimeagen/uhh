#add_subdirectory(comms)

#list(APPEND EXTRA_LIBS system-commands)

# add the executable
add_executable(uhh uhh.cpp)

# find_package(Threads REQUIRED)

# target_link_libraries(vim-with-me PUBLIC
#    ${EXTRA_LIBS}
#    Threads::Threads
#)

# configure_file(vim-with-me.h.in vim-with-me.h)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED True)

target_include_directories(uhh PUBLIC
    "${PROJECT_BINARY_DIR}"
)

target_include_directories(uhh PRIVATE
    "${PROJECT_LIB_FOLDER}"
)


