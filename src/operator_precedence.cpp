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
                case '#': return 1;
                default: throw UNKNOW_ERROR;
            }
        case ')':
            switch (second)
            {
                case '+': return 1;
                case '*': return 1;
                case '(': throw NOTALLOW_ERROR;
                case ')': return 1;
                case 'i': throw NOTALLOW_ERROR;
                case '#': return 1;
                default: throw UNKNOW_ERROR;
            }
        case 'i':
            switch (second)
            {
                case '+': return 1;
                case '*': return 1;
                case '(': throw NOTALLOW_ERROR;
                case ')': return 1;
                case 'i': throw NOTALLOW_ERROR;
                case '#': return 1;
                default: throw UNKNOW_ERROR;
            }
        case '#':
            switch (second)
            {
                case '+': return -1;
                case '*': return -1;
                case '(': return -1;
                case ')': return -1;
                case 'i': return -1;
                case '#': return 0;
                default: throw UNKNOW_ERROR;
            }
        default: throw UNKNOW_ERROR;
        }
    }


    const char reduced(std::string str)
    {
        if (str.size() == 1) 
        {
            switch (str.at(0))
            {
                case 'i': return 'F';
                case 'F': return 'T';
                case 'T': return 'E';
                default: throw REDUCED_ERROR;
            }
        }
        else if (str.size() == 3)
        {
            if (str.at(1) == '*')
            {
                if (str.at(0) == 'T' || str.at(0) == 'F')
                {
                    if (str.at(2) == 'F') return 'T';
                }
            }
            else if (str.at(1) == '+')
            {
                if (str.at(0) == 'E' || str.at(0) == 'T' || str.at(0) == 'F')
                {
                    if (str.at(2) == 'T' || str.at(2) == 'F') return 'E';
                }
            }
            else if (str.at(0) == '(' || str.at(2) == ')')
            {
                if (str.at(1) == 'E' || str.at(1) == 'T' || str.at(1) == 'F') return 'F';
            }
        }

        throw REDUCED_ERROR;
    }
} // namespace Compiler