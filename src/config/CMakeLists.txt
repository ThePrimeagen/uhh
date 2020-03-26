add_library(config-class config.cpp)

list(APPEND EXTRA_LIBS stdc++fs)
list(APPEND EXTRA_LIBS git2::git2)

target_link_libraries(config-class PUBLIC
    ${EXTRA_LIBS}
)

target_include_directories(config-class INTERFACE
    ${CMAKE_CURRENT_SOURCE_DIR}
)

target_include_directories(config-class PRIVATE
    ${PROJECT_LIB_FOLDER}
)

