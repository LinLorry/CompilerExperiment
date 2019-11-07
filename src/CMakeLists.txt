add_library(Grammar grammar.cpp)

link_libraries(Grammar)

add_subdirectory(input_output)

add_subdirectory(lexical_analysis)