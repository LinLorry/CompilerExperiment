#ifndef COMPILER_OPERATOR_PRECEDENCE
#define COMPILER_OPERATOR_PRECEDENCE

#include <string>

namespace Compiler
{
    static const char *const UNKNOW_ERROR = "不认识的运算符";
    static const char *const ILLEGAL_OPEN_BRACKET = "非法左括号";
    static const char *const MISSING_OPERATOR = "缺少运算符";
    static const char *const ILLEGAL_CLOSE_BRACKET = "非法右括号";
    static const char *const MISSING_EXPRESSION = "缺少表达式";

    int orecednece_judgment(int first, int second);

    const char reduced(std::string str);
} // namespace Compiler


#endif // COMPILER_OPERATOR_PRECEDENCE