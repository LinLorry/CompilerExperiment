#ifndef COMPILER_GRAMMAR_H
#define COMPILER_GRAMMAR_H

#include <set>
#include <string>
#include <map>

namespace Compiler
{
    typedef char VT_TYPE;
    typedef char VN_TYPE;
    typedef std::map<VN_TYPE, std::set<std::string>> Production;
    typedef Production::iterator ProIterator;
    typedef std::pair<VN_TYPE, std::set<std::string>> ProPair;

    class grammar
    {
    private:
        std::set<VT_TYPE> vt;
        std::set<VN_TYPE> vn;

        VN_TYPE start;

        Production production;
    public:
        grammar();
        ~grammar();

        inline void set_start(const VN_TYPE s);

        inline void add_vt(const VT_TYPE elem);

        inline void add_vn(const VN_TYPE elem);

        inline void remove_vn(const VN_TYPE elem);

        void add_production(const VT_TYPE begin, const std::string & end);

        inline const VN_TYPE get_start() const;

        inline std::set<VT_TYPE> & get_vt();

        inline const std::set<VT_TYPE> & get_vt() const;

        inline std::set<VN_TYPE> & get_vn();

        inline const std::set<VN_TYPE> & get_vn() const;

        inline Production & get_production();

        inline const Production & get_production() const;

        inline bool is_vt(char ch) const;

        inline bool is_vn(char ch) const;

        void construct_LL1();

    private:
        void eliminate_left_recursion();

        void eliminate_one_left_recursion(const VN_TYPE elem);

        void eliminate_indirect_recursion(const VN_TYPE i, const VN_TYPE j);

        void eliminate_unneed_vn();

        void eliminate_unneed_vn(const VN_TYPE elem, std::set<VN_TYPE> & s);

        void extract_left_divisor();

        void extract_one_left_divisor(const VN_TYPE elem);

        const VN_TYPE get_unuse_vn() const;    
    };

    void grammar::set_start(const VN_TYPE s)
    {
        start = s;
    }

    void grammar::add_vt(const VT_TYPE elem) { vt.insert(elem); }

    void grammar::add_vn(const VN_TYPE elem) { vn.insert(elem); }

    void grammar::remove_vn(const VN_TYPE elem) { vn.erase(elem); production.erase(elem); }

    const VN_TYPE grammar::get_start() const { return start; }

    std::set<VT_TYPE> & grammar::get_vt() { return vt; }

    const std::set<VT_TYPE> & grammar::get_vt() const { return vt; }

    std::set<VN_TYPE> & grammar::get_vn() { return vn; }

    const std::set<VN_TYPE> & grammar::get_vn() const { return vn; }

    Production & grammar::get_production() { return production; }

    const Production & grammar::get_production() const { return production; }

    bool grammar::is_vt(char ch) const { return vt.find(ch) != vt.cend(); }

    bool grammar::is_vn(char ch) const { return vn.find(ch) != vn.cend(); }

} // namespace Compiler

#endif // COMPILER_GRAMMAR_H

