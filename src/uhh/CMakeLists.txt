add_library(uhh-class uhh.cpp)

list(APPEND EXTRA_LIBS stdc++fs)
list(APPEND EXTRA_LIBS git2::git2)

target_link_libraries(uhh-class PUBLIC
    ${EXTRA_LIBS}
)

target_include_directories(uhh-class INTERFACE
    ${CMAKE_CURRENT_SOURCE_DIR}
)

target_include_directories(uhh-class PRIVATE
    ${PROJECT_LIB_FOLDER}
)

