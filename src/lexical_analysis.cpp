#include <compiler/lexical_analysis.h>

using std::string;

namespace Compiler
{
    LexicalAnalysisMachine::LexicalAnalysisMachine(const char *filename) 
        : fin(filename, std::ios_base::in), p(nullptr), result(ERROR) { }
    LexicalAnalysisMachine::LexicalAnalysisMachine(const std::string &filename)
        : fin(filename, std::ios_base::in), p(nullptr), result(ERROR) { }

    const bool LexicalAnalysisMachine::next()
    {
        skipblank();

        if (p == nullptr || *p == 0) 
        {
            result = END;
            token = "#";
            return false;
        }

        tokenInit();

        if (isalpha(*p))
        {
            add();
            while (isalnum(*p))
                add();

            if (iskeyword()) result = KEY;
            else result = IDENTIFICATION;
        }
        else if (*p == '0')
        {
            add();
            if (*p == 'x' || *p == 'X')
            {
                add();
                if (ishex(*p))
                {
                    add();
                    while (ishex(*p))
                        add();
                    result = HEX;
                }
                else {
                    add();
                    result = ERROR;
                }
            }
            else if (isdigit(*p))
            {
                if (*p != '0' && *p != '8' && *p != '9')
                {
                    add();
                    while (isoct(*p))
                        add();
                    result = OCT;
                }
                else {
                    add();
                    result = ERROR;
                }
            }
            else result = DIGUT;        
        }
        else if (isdigit(*p))
        {
            add();
            while (isdigit(*p))
                add();
            result = DIGUT;
        }
        else add(), result = KEY;

        return true;
    } 

    const bool LexicalAnalysisMachine::iskeyword() const
    {
        switch (token.at(0))
        {
        case 'd': return token.compare("do") == 0;
        case 'e': return token.compare("else") == 0;
        case 'i': return token.compare("if") == 0;
        case 't': return token.compare("then") == 0;
        case 'w': return token.compare("where") == 0;   
        default:  return false;
        }
    }

    void LexicalAnalysisMachine::skipblank()
    {
        if (p != nullptr)
            while (*p < 33 && *p > 0) ++p;
        
        while ((p == nullptr || *p == 0) && getline(fin, tmp))
        {
            if (tmp.empty()) continue;
            tmp.erase(0,tmp.find_first_not_of(" "));
            if (tmp.empty()) continue;
            tmp.erase(tmp.find_last_not_of(" ") + 1);

            p = tmp.c_str();

            while (*p < 33 && *p > 0) ++p;
        }
    }

} // namespace Compiler
