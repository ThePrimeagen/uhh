add_library(uhh-term terminal.cpp prompt.cpp)

target_include_directories(uhh-term PRIVATE
    "${CMAKE_SOURCE_DIR}/src"
)

add_executable(term-demo ui.cpp)
target_link_libraries(term-demo PUBLIC
    uhh-term
)
target_include_directories(term-demo PRIVATE
    "${CMAKE_SOURCE_DIR}/src"
)