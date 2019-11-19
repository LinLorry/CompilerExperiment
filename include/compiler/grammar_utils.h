#ifndef COMPILER_UTILS_H
#define COMPILER_UTILS_H

#include <compiler/grammar.h>

namespace Compiler
{
    grammar create_grammar_by_file(char * filename);

    void deal_with_left(grammar & g, const std::string & right, bool & first);

    void deal_with_right(grammar & g, const VN_TYPE vn, const std::string & left);
} // namespace Compiler

#endif // COMPILER_UTILS_H
