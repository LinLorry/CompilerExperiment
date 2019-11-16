#include <compiler/grammar_utils.h>
#include <iostream>

using std::cout;
using std::endl;
using std::set;
using std::string;

using namespace Compiler;

int main(int argc, char *argv[])
{
    if (argc != 2)
    {
        printf("Usage: %s filename\n", argv[0]);
        exit(EXIT_FAILURE);
    }
    grammar g = create_grammar_by_file(argv[1]);
    
    g.eliminate_left_recursion();

    cout << "The result:" << endl;
    
    for (VN_TYPE vn : g.get_vn())
    {
        cout << vn << " ";
    }

    cout << endl;

    for (ProPair pair: g.get_production())
    {
        string tmp;
        tmp += pair.first;
        tmp += "->";

        for (string s : pair.second)
        {
            tmp += s + "|";
        }
        tmp.back() = ';';
        cout << tmp << endl;
    }
}