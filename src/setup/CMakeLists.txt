add_library(setup setup.cpp setup.h)

list(APPEND EXTRA_LIBS stdc++fs)
list(APPEND EXTRA_LIBS git2)

target_link_libraries(setup PUBLIC
    ${EXTRA_LIBS}
)

target_include_directories(setup INTERFACE
    ${CMAKE_CURRENT_SOURCE_DIR}
)

target_include_directories(setup PRIVATE
    ${PROJECT_LIB_FOLDER}
)

