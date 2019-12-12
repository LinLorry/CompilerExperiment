#ifndef COMPILER_OPERATOR_PRECEDENCE
#define COMPILER_OPERATOR_PRECEDENCE

#include <string>

namespace Compiler
{
    static const char *const UNKNOW_ERROR = "Unknow operator!";
    static const char *const NOTALLOW_ERROR = "Not allow operator!";
    static const char *const REDUCED_ERROR = "REDUCE ERROR!";

    int orecednece_judgment(int first, int second);

    const char reduced(std::string str);
} // namespace Compiler


#endif // COMPILER_OPERATOR_PRECEDENCE