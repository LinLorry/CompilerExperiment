#include <compiler/grammar_utils.h>

#include <fstream>
#include <string>
#include <iostream>

namespace Compiler
{
    grammar create_grammar_by_file(char * filename)
    {
        grammar g;
        std::ifstream fin = std::ifstream(filename, std::ios_base::in);
        std::string s;
        bool first= true;
    
        if (!fin.is_open())
        {
            std::cout << "Can't open " << filename << std::endl;
            exit(EXIT_FAILURE);
        }

        while (std::getline(fin, s))
        {
            size_t index = s.find("->");

            if (index == std::string::npos)
            {
                std::cerr << "ERROR::" << s << "::NOT\"->\"." << std::endl;
                continue;
            }

            if (*(--s.cend()) != ';')
            {
                std::cerr << "ERROR::" << s << "::NOT \";\"." << std::endl;
            }

            s.erase(s.size() - 1);
            
            std::string left = s.substr(0, index);
            std::string right = s.substr(index+2);

            deal_with_left(g, left, first);
            deal_with_right(g, right);

            g.add_production(left.at(0), right);
        }

        return g;
    }

    void deal_with_left(grammar & g, const std::string & right, bool & first)
    {
        if (first)
        {
            first = false;
            g.set_start(right.at(0));
        }

        g.add_vn(right.at(0));
    }

    void deal_with_right(grammar & g, const std::string & left)
    {
        for (char ch : left)
        {
            if (isupper(ch))
                g.add_vn(ch);
            else
                g.add_vt(ch);
        }
    }   
} // namespace Compiler


