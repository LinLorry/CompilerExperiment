#ifndef COMPILER_EXCEPTION_H
#define COMPILER_EXCEPTION_H

#include <exception>

namespace Compiler
{
    class ReduceException : std::exception
    {
    private:
    public:
        ReduceException(/* args */)
        {

        }
        ~ReduceException()
        {
            
        }
    };
} // namespace Compiler


#endif // COMPILER_EXCEPTION