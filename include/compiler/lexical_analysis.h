#ifndef COMPILER_LEXICAL_ANALYSIS_H
#define COMPILER_LEXICAL_ANALYSIS_H

#include <string>
#include <fstream>

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
        END
    };

    class LexicalAnalysisMachine
    {
    private:
        std::ifstream fin;
        std::string tmp;
        std::string token;
        const char *p;
        LEXICAL_RESULT result;

    public:
        LexicalAnalysisMachine(const char *filename);
        LexicalAnalysisMachine(const std::string &filename);

        const bool next();
        inline const LEXICAL_RESULT get_result() const;
        inline const std::string &get_token() const;

        inline const bool isOpen() const;

    private:

        inline void skipblank();

        inline void tokenInit();

        inline void add();

        const bool iskeyword() const;

    private:
        inline static const bool isoct(const char ch);

        inline static const bool ishex(const char ch);
    };

    const LEXICAL_RESULT LexicalAnalysisMachine::get_result() const
    {
        return result;
    }

    const std::string &LexicalAnalysisMachine::get_token() const
    {
        return token;
    }

    void LexicalAnalysisMachine::tokenInit()
    {
        token.erase(token.cbegin(), token.cend());
    }

    void LexicalAnalysisMachine::add()
    {
        token += *p++;
    }

    const bool LexicalAnalysisMachine::isOpen() const
    {
        return fin.is_open();
    }


    const bool LexicalAnalysisMachine::isoct(const char ch)
    {
        return ch < '8' && ch > '0' - 1; // '0' 48
    }

    const bool LexicalAnalysisMachine::ishex(const char ch)
    {
        return (ch < '9' + 1 && ch > '0' + 1) || (ch < 'g' && ch > 'a' - 1); // '0' 48 '9' 57 'a' 97
    }

} // namespace Compiler



#endif // COMPILER_LEXICAL_ANALYSIS_H