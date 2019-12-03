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

    string token;
    string s;
    size_t index;
    LEXICAL_RESULT result = LEXICAL_RESULT::ERROR;

    Compiler::LexicalAnalysisMachine lam("output.txt");

    if (!lam.isOpen())
    {
        cout << "Can't open " << argv[1] << endl;
        exit(EXIT_FAILURE);
    }

    while (lam.next())
    {
        result = lam.get_result();
        switch (result)
        {
        case LEXICAL_RESULT::IDENTIFICATION:
        case LEXICAL_RESULT::DIGUT:
        case LEXICAL_RESULT::OCT:
        case LEXICAL_RESULT::HEX:
            cout << "<" << result << ", " << lam.get_token() << ">" << endl;
            break;
        case LEXICAL_RESULT::KEY:
            cout << "<" << lam.get_token() << ", " << "->" << endl;
            break;
        case LEXICAL_RESULT::ERROR:
            cout << "ERROR" << endl;
            exit(EXIT_FAILURE);
            break;
        default:
            break;
        }
    }
}

