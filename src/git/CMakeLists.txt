add_library(uhh-git uhh-git.cpp)

list(APPEND EXTRA_LIBS stdc++fs)
list(APPEND EXTRA_LIBS git2::git2)

target_link_libraries(uhh-git PUBLIC
    ${EXTRA_LIBS}
)

target_include_directories(uhh-git INTERFACE
    ${CMAKE_CURRENT_SOURCE_DIR}
)

target_include_directories(uhh-git PRIVATE
    ${PROJECT_LIB_FOLDER}
)


