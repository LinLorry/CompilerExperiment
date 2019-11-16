#include <compiler/grammar.h>

namespace Compiler
{
    grammar::grammar() : vn(), vt(), production() { }

    grammar::~grammar() { }

    void grammar::add_production(const VN_TYPE begin, const std::string & end)
    {
        ProIterator iter = production.find(begin);

        if (iter != production.end())
        {
            iter->second.insert(end);
        }
        else
        {
            std::set<std::string> *ends = new std::set<std::string>();
            ends->insert(end);
            production.insert(*(new ProPair(begin, *ends)));
        }
    }
} // namespace Compiler
