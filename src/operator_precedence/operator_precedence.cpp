#include <compiler/grammar_utils.h>
#include <compiler/lexical_analysis.h>
#include <compiler/operator_precedence.h>

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

void dealWithReduceError(const string & str, const grammar & g);

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
    string stack = "#";
    string str_tmp;
    bool next = 0;

    char ch;

    if (!machine.isOpen())
    {
        cerr << "Input filename isn't exist!" << endl;
        exit(EXIT_FAILURE);
    }

    do
    {
        next = machine.next();
        str_tmp += machine.get_token();

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
            cerr << "出现无效字符： "<< machine.get_token() << endl;
            continue;          
        }

        if (!next && str_tmp.size() == 1) break;

        if (token == ';') token = '#';
        
        if (stack.at(k) == '#' || g.is_vt(stack.at(k))) index = k;
        else index = k - 1;

        try
        {
            while (orecednece_judgment(stack.at(index), token) == 1)
            {
                do
                {
                    tmp = stack.at(index);
                    if (stack.at(index - 1) == '#' || g.is_vt(stack.at(index - 1))) --index;
                    else index -= 2;
                } 
                while (orecednece_judgment(stack.at(index), tmp) != -1);

                N = g.fuzzy_reduced(stack.substr(index + 1, k));
                stack.erase(index + 1, k - index - 1);
                k = index + 1;
                stack.at(k) = N; 
            }
        } 
        catch (const char *e) 
        {
            cerr << e << endl;
        }
        catch (const Compiler::ReduceException e)
        {
            dealWithReduceError(stack.substr(index + 1, k), g);
        }

        try
        {
            if (orecednece_judgment(stack.at(index), token) != 1)
            {
                ++k;
                stack += token;
            }
            else
            {
                cerr << "ERROR::AT '" << machine.get_token() << "'\n";
            }
        }
        catch (const char *e) 
        {
            cerr << e << endl;
        }

        if (token == '#')
        {
            cout << "字符串： " << str_tmp << " 判断结束" << endl;
            str_tmp = "";
        }

    } while (next);

    cout << stack << endl;
}

void dealWithReduceError(const string & str, const grammar & g)
{
    size_t i, j;
    if (
        (i = str.find('(')) != str.npos && 
        (j = str.find(')')) != str.npos && 
        j - i == 2 && 
        !g.is_vn(str.at(i+1))
    ) cerr << "括号间无表达式" << endl;
    else if (
        (i = str.find('i')) != str.npos &&
        (
            (i > 0 && g.is_vn(str.at(i - 1))) ||
            (i < str.size() - 1 && g.is_vn(str.at(i + 1)))
        )
    ) cerr << "表达式间无运算符联结" << endl;
    else if (
        (
            (i = str.find('*')) != str.npos || 
            (i = str.find('+')) != str.npos
        ) &&
        (
            i == 0 ||
            i == str.size() - 1 ||
            !g.is_vn(str.at(i - 1)) || 
            !g.is_vn(str.at(i + 1))
        )
    ) cerr << "缺表达式" << endl;
}