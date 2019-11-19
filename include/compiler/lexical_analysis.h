#ifndef COMPILER_LEXICAL_ANALYSIS_H
#define COMPILER_LEXICAL_ANALYSIS_H

#include <string>

namespace Compiler
{
    enum LEXICAL_RESULT
    {
        ERROR = -1,
        IDENTIFICATION = 0,
        DIGUT,
        OCT,
        HEX,
        KEY,
        EMPTY
    };

    LEXICAL_RESULT scan(const char *buffer, std::string & s, size_t *index);
} // namespace Compiler



#endif // COMPILER_LEXICAL_ANALYSIS_H