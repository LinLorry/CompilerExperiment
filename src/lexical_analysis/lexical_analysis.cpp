#include <fstream>
#include <iostream>
#include <string>

#include <compiler/lexical_analysis.h>

using std::ifstream;
using std::cout;
using std::endl;
using std::string;
using Compiler::LEXICAL_RESULT;

int main(int argc, char *argv[])
{
    if (argc != 2)
    {
        printf("Usage: %s filename\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    ifstream fin = ifstream(argv[1], std::ios_base::in);
    string token;
    string s;
    size_t index;
    LEXICAL_RESULT result = LEXICAL_RESULT::ERROR;

    if (!fin.is_open())
    {
        cout << "Can't open " << argv[1] << endl;
        exit(EXIT_FAILURE);
    }

    while (getline(fin, token))
    {
        index = 0;
        result = LEXICAL_RESULT::ERROR;

        while (result != LEXICAL_RESULT::EMPTY)
        {
            result = Compiler::scan(token.c_str() + index, s, &index);
            switch (result)
            {
            case LEXICAL_RESULT::IDENTIFICATION:
            case LEXICAL_RESULT::DIGUT:
            case LEXICAL_RESULT::OCT:
            case LEXICAL_RESULT::HEX:
                cout << "<" << result << ", " << s << ">" << endl;
                break;
            case LEXICAL_RESULT::KEY:
                cout << "<" << s << ", " << "->" << endl;
                break;
            case LEXICAL_RESULT::EMPTY:
                break;
            case LEXICAL_RESULT::ERROR:
                cout << "ERROR::" << token << endl;
                exit(EXIT_FAILURE);
                break;
            default:
                break;
            }
        }
    }
}

