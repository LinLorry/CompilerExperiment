add_library(Compiler grammar.cpp grammar_utils.cpp lexical_analysis.cpp)

link_libraries(Compiler)

add_subdirectory(lexical_analysis)

add_subdirectory(structure_LL1)