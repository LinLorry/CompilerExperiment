#include <grammar.h>

namespace Compile
{
    grammar::grammar() : vn(), vt(), production() { }

    grammar::~grammar() { }

    void grammar::add_production(const VN_TYPE begin, const string & end)
    {
        ProIterator iter = production.find(begin);

        if (iter != production.end())
        {
            iter->second.insert(end);
        }
        else
        {
            set<string> *ends = new set<string>();
            ends->insert(end);
            production.insert(*(new ProPair(begin, *ends)));
        }
    }
} // namespace Compile
