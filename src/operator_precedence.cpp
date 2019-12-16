#include <compiler/operator_precedence.h>

namespace Compiler
{
    int orecednece_judgment(int first, int second)
    {
        switch (first)
        {
        case '+':
            switch (second)
            {
                case '+': return 1;
                case '*': return -1;
                case '(': return -1;
                case ')': return 1;
                case 'i': return -1;
                case '#': return 1;
                default: throw UNKNOW_ERROR;
            }
        case '*':
            switch (second)
            {
                case '+': return 1;
                case '*': return 1;
                case '(': return -1;
                case ')': return 1;
                case 'i': return -1;
                case '#': return 1;
                default: throw UNKNOW_ERROR;
            }
        case '(':
            switch (second)
            {
                case '+': return -1;
                case '*': return -1;
                case '(': return -1;
                case ')': return 0;
                case 'i': return -1;
                case '#': throw ILLEGAL_OPEN_BRACKET;
                default: throw UNKNOW_ERROR;
            }
        case ')':
            switch (second)
            {
                case '+': return 1;
                case '*': return 1;
                case '(': throw MISSING_OPERATOR;
                case ')': return 1;
                case 'i': throw MISSING_OPERATOR;
                case '#': return 1;
                default: throw UNKNOW_ERROR;
            }
        case 'i':
            switch (second)
            {
                case '+': return 1;
                case '*': return 1;
                case '(': throw MISSING_OPERATOR;
                case ')': return 1;
                case 'i': throw MISSING_OPERATOR;
                case '#': return 1;
                default: throw UNKNOW_ERROR;
            }
        case '#':
            switch (second)
            {
                case '+': return -1;
                case '*': return -1;
                case '(': return -1;
                case ')': throw ILLEGAL_CLOSE_BRACKET;
                case 'i': return -1;
                case '#': return 0;
                default: throw MISSING_EXPRESSION;
            }
        default: throw UNKNOW_ERROR;
        }
    }
} // namespace Compiler