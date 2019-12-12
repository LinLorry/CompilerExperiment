#include <compiler/grammar_utils.h>
#include <compiler/lexical_analysis.h>
#include <compiler/operator_precedence.h>

#include <cstring>
#include <iostream>
#include <string>

using std::cerr;
using std::cout;
using std::endl;
using std::string;

using Compiler::grammar;
using Compiler::LexicalAnalysisMachine;
using Compiler::LEXICAL_RESULT;
using Compiler::orecednece_judgment;

int main(int argc, char *argv[]) 
{
    if (argc != 3)
    {
        cerr << "Usage: " << argv[0] << "\"grammar filename\" \"input filename\"" << endl; 
        exit(EXIT_FAILURE);
    }
    grammar g = Compiler::create_grammar_by_file(argv[1]);
    LexicalAnalysisMachine machine(argv[2]);
    unsigned k = 0;
    unsigned index = 0;
    char token, tmp, N;
    string s = "#";
    short next = 0;

    if (machine.isOpen())
    {
        cerr << "Input filename isn't exist!" << endl;
        exit(EXIT_FAILURE);
    }

    do
    {
        machine.next();

        switch (machine.get_result())
        {
        case LEXICAL_RESULT::IDENTIFICATION:
        case LEXICAL_RESULT::DIGUT:
        case LEXICAL_RESULT::OCT:
        case LEXICAL_RESULT::HEX: token = 'i'; break;
        case LEXICAL_RESULT::KEY: 
        case LEXICAL_RESULT::END: token = machine.get_token().at(0); break;
        case LEXICAL_RESULT::ERROR:
        default:
            cerr << "ERROR::INPUT IS NOT VAILD\n";
            while (machine.next() && machine.get_token().at(0) != ';') ;
            if (machine.get_token().at(0) == ';')
            {
                s += '#';
                token = ';';
            }
            else token = '#';

            continue;            
        }

        if (token == ';')
        {
            token = '#';
            next = 1;
        }
        
        if (s.at(k) == '#' || g.is_vt(s.at(k))) index = k;
        else index = k - 1;

        try
        {
            while (orecednece_judgment(s.at(index), token) == 1)
            {
                do
                {
                    tmp = s.at(index);
                    if (s.at(index - 1) == '#' || g.is_vt(s.at(index - 1))) --index;
                    else index -= 2;
                    
                } 
                while (orecednece_judgment(s.at(index), tmp) != -1);

                N = Compiler::reduced(s.substr(index + 1, k));
                s.erase(index + 1, k - index - 1);
                k = index + 1;
                s.at(k) = N; 
            }
        } 
        catch (const char *e) 
        {
            cerr << "ERROR::" << e << "::AT '" << machine.get_token() << "'\n";
            
            while (machine.get_token().at(0) != ';' && machine.next()) ;
            if (machine.get_token().at(0) == ';')
            {
                s += '#';
                token = ';';
            }
            else token = '#';
            next = 0;
            ++k;

            continue; 
        }
        
        if (orecednece_judgment(s.at(index), token) != 1)
        {
            ++k;
            s += token;
        }
        else
        {
            cerr << "ERROR::AT '" << machine.get_token() << "'\n";
            while (machine.get_token().at(0) != ';' && machine.next()) ;
            if (machine.get_token().at(0) == ';')
            {
                s += '#';
                token = ';';
            }
            else token = '#';
            next = 0;
            ++k;

            continue; 
        }
        

        if (next) 
        {
            token = ';';
            next = 0;
        }
        
    } while (token != '#');

    cout << s << endl;
}