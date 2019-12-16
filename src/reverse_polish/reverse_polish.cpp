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

struct tuple
{
    Compiler::LEXICAL_RESULT result;
    std::string value;
};

void transForPolish(const string & str,std::stack<tuple> & stack);

int computePolish(std::stack<tuple> & stack);

void dealWithReduceError(const string & str, const grammar & g);

int main(int argc, char *argv[]) 
{
    // if (argc != 3)
    // {
    //     cerr << "Usage: " << argv[0] << "\"grammar filename\" \"input filename\"" << endl; 
    //     exit(EXIT_FAILURE);
    // }
    // grammar g = Compiler::create_grammar_by_file(argv[1]);
    // LexicalAnalysisMachine machine(argv[2]);
    grammar g = Compiler::create_grammar_by_file("grammar.txt");
    LexicalAnalysisMachine machine("input.txt");
    unsigned k = 0;
    unsigned index = 0;
    char token, tmp, N;
    string stack = "#";
    std::stack<tuple> polish;
    string str_tmp;
    bool next = 0;

    char ch;

    // if (!machine.isOpen())
    // {
    //     cerr << "Input filename isn't exist!" << endl;
    //     exit(EXIT_FAILURE);
    // }

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
                    polish.push({machine.get_result(), machine.get_token()});
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
            cout << "字符串： " << str_tmp << " 判断结束,计算结果为： " << computePolish(polish) << endl;
            str_tmp = "";
        }

    } while (next);
}

void transForPolish(const string & str,std::stack<tuple> & stack)
{
    static const tuple plus = { LEXICAL_RESULT::KEY, "+"};
    static const tuple mut = { LEXICAL_RESULT::KEY, "*"};
    std::stack<tuple> tmp;

    if (str.size() == 3)
    {
        if (str.at(0) == '(' && str.at(2) == ')')
        {
            stack.pop();
            while (stack.top().value.at(0) != '(')
            {
                tmp.push(stack.top());
                stack.pop();
            }

            stack.pop();

            while (!tmp.empty())
            {
                stack.push(tmp.top());
                tmp.pop();
            }
            
            return ;
        }
        else if (str.at(1) == '+')
        {

            while (stack.top().value.at(0) != '+')
            {   
                tmp.push(stack.top());
                stack.pop();
            }

            stack.pop();

            while (!tmp.empty())
            {
                stack.push(tmp.top());
                tmp.pop();
            }

            stack.push(plus);


            return ;
        }
        else if (str.at(1) == '*')
        {
            while (stack.top().value.at(0) != '*')
            {   
                tmp.push(stack.top());
                stack.pop();
            }

            stack.pop();

            while (!tmp.empty())
            {
                stack.push(tmp.top());
                tmp.pop();
            }

            stack.push(mut);

            return ;
        }
    }
    else if (str.size() == 1)
    {
        return ;
    }
    throw Compiler::ReduceException();
}

int computePolish(std::stack<tuple> & stack)
{
    bool first = true;
    int t1 = 0, t2 = 0;

    std::stack<int> num_stack;
    std::stack<tuple> re_stack;

    while (!stack.empty())
    {
        re_stack.push(stack.top());
        stack.pop();
    }

    while (!re_stack.empty())
    {
        if (re_stack.top().result == LEXICAL_RESULT::KEY)
        {
            t1 = num_stack.top();
            num_stack.pop();
            t2 = num_stack.top();
            num_stack.pop();
            if (re_stack.top().value.at(0) == '*')
            {
                num_stack.push(t1 * t2);
            }
            else if (re_stack.top().value.at(0) == '+')
            {
                num_stack.push(t1 + t2);
            }
        }
        else 
        {
            switch (re_stack.top().result)
            {
            case LEXICAL_RESULT::IDENTIFICATION: throw "不支持变量！";
            case LEXICAL_RESULT::DIGUT: num_stack.push(std::stoi(re_stack.top().value, nullptr,10)); break;
            case LEXICAL_RESULT::OCT: num_stack.push(std::stoi(re_stack.top().value, nullptr,8)); break;
            case LEXICAL_RESULT::HEX: num_stack.push(std::stoi(re_stack.top().value, nullptr,16)); break;
            case LEXICAL_RESULT::END:
            case LEXICAL_RESULT::ERROR:
            default: throw "波兰表达式错误！";
            }
        }
        re_stack.pop();
    }
    return num_stack.top();
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