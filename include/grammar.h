#ifndef COMPILE_GRAMMAR_H
#define COMPILE_GRAMMAR_H

#include <set>
#include <string>
#include <map>

using std::set;
using std::string;
using std::pair;
using std::map;

namespace Compile
{
    typedef char VN_TYPE;
    typedef char VT_TYPE;
    typedef map<VN_TYPE, set<string>> Production;
    typedef Production::iterator ProIterator;
    typedef pair<VN_TYPE, set<string>> ProPair;

    class grammar
    {
    private:
        set<VN_TYPE> vn;
        set<VT_TYPE> vt;

        VN_TYPE start;

        Production production;
    public:
        grammar();
        ~grammar();

        inline void set_start(const VN_TYPE s);

        inline void add_vn(const VN_TYPE elem);

        inline void add_vt(const VT_TYPE elem);

        void add_production(const VN_TYPE begin, const string & end);

        inline const VN_TYPE get_start() const;

        inline const set<VT_TYPE> & get_vt() const;

        inline const set<VN_TYPE> & get_vn() const;

        inline const Production & get_production() const;
    };

    void grammar::set_start(const VN_TYPE s)
    {
        start = s;
    }

    void grammar::add_vn(const VN_TYPE elem) { vn.insert(elem); }

    void grammar::add_vt(const VT_TYPE elem) { vt.insert(elem); }

    const VN_TYPE grammar::get_start() const { return start; }

    const set<VT_TYPE> & grammar::get_vt() const { return vt; }

    const set<VN_TYPE> & grammar::get_vn() const { return vn; }

    const Production & grammar::get_production() const { return production; }

} // namespace Compile

#endif // COMPILE_GRAMMAR_H

