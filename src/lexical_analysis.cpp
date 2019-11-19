#include <compiler/lexical_analysis.h>

using std::string;

namespace Compiler
{
    inline bool isoct(const char c)
    {
        return c < '8' && c > '0' - 1; // '0' 48
    }

    inline bool ishex(const char c)
    {
        return (c < '9' + 1 && c > '0' + 1) || (c < 'g' && c > 'a' - 1); // '0' 48 '9' 57 'a' 97
    }

    inline const char *skipblank(const char *p)
    {
        while (*p < 33 && *p != 0) ++p;

        return p;
    }

    const bool iskeyword(const string & s)
    {
        switch (s.at(0))
        {
        case 'd': return s.compare("do") == 0;
        case 'e': return s.compare("else") == 0;
        case 'i': return s.compare("if") == 0;
        case 't': return s.compare("then") == 0;
        case 'w': return s.compare("where") == 0;   
        default:  return false;
        }
    }

    LEXICAL_RESULT scan(const char *buffer, string & s, size_t *index)
    {
        const char *p = buffer;
        p = skipblank(p);
        if (*p == 0) return EMPTY;

        LEXICAL_RESULT result;
        s.erase(s.cbegin(), s.cend());
        s += *p;

        if (isalpha(*p))
        {
            while (isalnum(*(++p)))
                s += *p;

            if (iskeyword(s)) result = KEY;
            else result = IDENTIFICATION;
        }
        else if (*p == '0')
        {
            if (*(++p) == 'x' || *p == 'X')
            {
                s += *p;
                if (ishex(*++p))
                {
                    s += *p;
                    while (ishex(*(++p)))
                        s += *p;
                    result = HEX;
                }
                else result = ERROR;
            }
            else if (isdigit(*p))
            {
                if (*p != '0' && *p != '8' && *p != '9')
                {
                    s += *p;
                    while (isoct(*++p))
                        s += *p;
                    result = OCT;
                }
                else result = ERROR;
            }
            else result = DIGUT;        
        }
        else if (isdigit(*p))
        {
            while (isdigit(*(++p)))
            {
                s += *p;
            }
            result = DIGUT;
        }
        else ++p, result = KEY;

        *index += (p - buffer);
        return result;
    }
} // namespace Compiler
