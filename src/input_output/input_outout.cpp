#include <fstream>
#include <iostream>

#include <grammar.h>

using std::ifstream;
using std::cout;
using std::endl;

using namespace Compile;

void deal_with_left(grammar & g, const string & right, bool & first);

void deal_with_right(grammar & g, const string & left);

int main(int argc, char *argv[]) 
{
    if (argc != 2)
    {
        printf("Usage: %s filename\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    grammar g;
    ifstream fin = ifstream(argv[1], std::ios_base::in);
    string s;
    bool first= true;
    
    if (!fin.is_open())
    {
        cout << "Can't open " << argv[1] << endl;
        exit(EXIT_FAILURE);
    }

    while (std::getline(fin, s))
    {
        size_t index = s.find("->");

        if (index == string::npos)
        {
            cout << "ERROR::" << s << "::NOT\"->\"." << endl;
            continue;
        }

        if (*(--s.cend()) != ';')
        {
            cout << "ERROR::" << s << "::NOT \";\"." << endl;
        }

        s.erase(s.size() - 1);
        
        string left = s.substr(0, index);
        string right = s.substr(index+2);

        deal_with_left(g, left, first);
        deal_with_right(g, right);

        g.add_production(left.at(0), right);
    }

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

void deal_with_left(grammar & g, const string & right, bool & first)
{
    if (first)
    {
        first = false;
        g.set_start(right.at(0));
    }

    g.add_vt(right.at(0));
}

void deal_with_right(grammar & g, const string & left)
{
    for (char ch : left)
    {
        if (isupper(ch))
            g.add_vt(ch);
        else
            g.add_vn(ch);
    }
}

