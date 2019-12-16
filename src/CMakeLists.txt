add_library(Compiler grammar.cpp grammar_utils.cpp lexical_analysis.cpp operator_precedence.cpp)

link_libraries(Compiler)

add_subdirectory(lexical_analysis)

add_subdirectory(structure_LL1)

add_subdirectory(operator_precedence)

add_subdirectory(reverse_polish)