#include <compiler/grammar_utils.h>
#include <compiler/lexical_analysis.h>
#include <compiler/operator_precedence.h>

#include <iostream>
#include <string>
#include <stack>

using std::cerr;
using std::cout;
using std::endl;
using std::string;

using Compiler::grammar;
using Compiler::LexicalAnalysisMachine;
using Compiler::LEXICAL_RESULT;
using Compiler::orecednece_judgment;

void transForPolish(const string & str,std::stack<string> & stack);

int computePolish(const string & str);

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
    std::stack<string> polish;
    string str_tmp;
    bool next = 0;

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
                const string t = stack.substr(index + 1, k);

                N = g.fuzzy_reduced(t);
                transForPolish(t, polish);
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

                if (token != '#') 
                {
                    switch (machine.get_result())
                    {
                    case LEXICAL_RESULT::IDENTIFICATION: throw "不支持变量！";
                    case LEXICAL_RESULT::KEY:
                    case LEXICAL_RESULT::DIGUT: polish.push(machine.get_token());break;
                    case LEXICAL_RESULT::OCT: polish.push(std::to_string(std::stoi(machine.get_token(), nullptr,8))); break;
                    case LEXICAL_RESULT::HEX: polish.push(std::to_string(std::stoi(machine.get_token(), nullptr,16))); break;
                    }
                }
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
            cout << "字符串： " << str_tmp << " 判断结束\n";

            if (polish.size() == 1)
                cout << "计算结果为：" << computePolish(polish.top()) << endl;
            else 
                cout << "波兰表达式有误！" << endl;

            while (!polish.empty())
                polish.pop();

            str_tmp = "";
        }

    } while (next);
}

void transForPolish(const string & str,std::stack<string> & stack)
{
    string tmp;

    if (str.size() == 3)
    {
        if (str.at(0) == '(' && str.at(2) == ')')
        {
            stack.pop();
            while (stack.top().at(0) != '(')
            {
                tmp += stack.top();
                stack.pop();
            }

            stack.pop();
            stack.push(tmp);            
            return ;
        }
        else if (str.at(1) == '+')
        {

            while (stack.top().at(0) != '+')
            {   
                tmp += stack.top();
                stack.pop();
            }

            stack.pop();

            tmp = stack.top() + " " + tmp + " +";
            stack.pop();
            stack.push(tmp);

            return ;
        }
        else if (str.at(1) == '*')
        {
            while (stack.top().at(0) != '*')
            {   
                tmp += stack.top();
                stack.pop();
            }

            stack.pop();

            tmp = stack.top() + " " + tmp + " *";
            stack.pop();
            stack.push(tmp);

            return ;
        }
    }
    else if (str.size() == 1)
    {
        return ;
    }
    throw Compiler::ReduceException();
}

int computePolish(const string & str)
{
    string tmp;
    std::stack<int> num;

    for (char ch : str)
    {
        if (ch == ' ')
        {
            if (tmp.at(0) == '+')
            {
                const int v1 = num.top();
                num.pop();
                const int v2 = num.top();
                num.pop();

                num.push(v1 + v2);
            }
            else if (tmp.at(0) == '*')
            {
                const int v1 = num.top();
                num.pop();
                const int v2 = num.top();
                num.pop();

                num.push(v1 * v2);
            }
            else
            {
                num.push(std::stoi(tmp, nullptr,10));
            }
            tmp = "";
        }
        else
        {
            tmp += ch;
        } 
    }
    const int v1 = num.top();
    num.pop();
    const int v2 = num.top();
    num.pop();

    if (tmp.at(0) == '+')
    {
        return v1 + v2;
    }
    else if (tmp.at(0) == '*')
    {
        return v1 * v2;
    }

    throw "波兰表达式错误！";
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