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

    void grammar::eliminate_left_recursion()
    {
        std::set<VN_TYPE>::const_iterator vn_i_citer;
        std::set<VN_TYPE>::const_iterator vn_j_citer;

        for (vn_i_citer = vn.cbegin(); vn_i_citer != vn.cend(); ++vn_i_citer)
        {
            for(vn_j_citer = vn.cbegin(); vn_j_citer != vn_i_citer; ++vn_j_citer)
            {
                eliminate_indirect_recursion(*vn_i_citer, *vn_j_citer);
            }
            eliminate_one_left_recursion(*vn_i_citer);
        }

        eliminate_unneed_vn();
    }

    void grammar::eliminate_unneed_vn()
    {
        std::set<VN_TYPE> s;
        std::set<VN_TYPE> unneed;
        this->eliminate_unneed_vn(start, s);

        std::set<VN_TYPE>::const_iterator iter;

        for (iter = vn.cbegin(); iter != vn.cend(); )
            if (s.find(*iter) == s.end())
            {
                production.erase(*iter);
                iter = vn.erase(iter);
            }
            else ++iter;
    }

    void grammar::extract_left_divisor()
    {
        for (VN_TYPE ch : vn)
            this->extract_one_left_divisor(ch);
    }

    const VN_TYPE grammar::get_unuse_vn() const
    {
        VN_TYPE result;
        for (result = 'A'; vn.find(result) != vn.end() && result <= 'Z'; ++result) ;
        if (result > 'Z')
            throw "Don't have other VT!";
        return result; 
    }

    void grammar::eliminate_indirect_recursion(const VN_TYPE i, const VN_TYPE j)
    {
        std::set<std::string> & i_set = production.at(i);
        std::set<std::string> & j_set = production.at(j);

        std::set<std::string>::const_iterator i_iter;
        std::set<std::string>::const_iterator j_iter;

        for(i_iter = i_set.cbegin(); i_iter != i_set.cend(); )
        {
            if (i_iter->at(0) == j)
            {
                for (j_iter = j_set.cbegin(); j_iter != j_set.cend(); ++j_iter)
                {
                    i_set.insert(*j_iter + i_iter->substr(1, i_iter->size()));
                }
                i_iter = i_set.erase(i_iter);
            }
            else ++i_iter;
        }
    }

    void grammar::eliminate_one_left_recursion(VN_TYPE elem)
    {
        std::set<std::string> & p = production.at(elem);
        std::set<std::string> new_set;
        std::set<std::string>::const_iterator iter;
        VN_TYPE other = 0;

        for (iter = p.cbegin(); iter != p.cend();)
        {
            if (iter->at(0) == elem) 
            {
                if (other == 0) 
                {
                    other = this->get_unuse_vn();
                    vn.insert(other);
                    vt.insert('@');
                    this->add_production(other, "@");
                }
                this->add_production(other, iter->substr(1, iter->size()) + other);
                iter = p.erase(iter);
            }
            else ++iter;
        }

        if (other != 0)
        {
            for (iter = p.cbegin(); iter != p.cend(); ++iter)
            {
                new_set.insert(*iter + other);
            }
            p.erase(p.begin(), p.end());
            p.insert(new_set.begin(), new_set.end());
        }
    }

    void grammar::eliminate_unneed_vn(const VN_TYPE elem, std::set<VN_TYPE> & s)
    {
        s.insert(elem);
        const std::set<std::string> & p = production.at(elem);
        for (std::string str : p)
            for (char ch : str)
                if (isupper(ch) && s.find(ch) == s.end())
                    this->eliminate_unneed_vn(ch ,s);
    }

    void grammar::extract_one_left_divisor(const VN_TYPE elem)
    {
        std::set<std::string> & this_production = production.at(elem);
        std::map<char, std::string> save_map;
        std::map<char, VN_TYPE> new_vn_map;

        for (auto iter = this_production.cbegin(); iter != this_production.cend();)
        {
            const char ch = iter->at(0);
            auto save_find = save_map.find(ch);

            if (save_find == save_map.end())
            {
                save_map.insert(std::pair<char, std::string>(ch, *iter));
                ++iter;
            }
            else
            {
                VN_TYPE other;
                auto map_iter = new_vn_map.find(ch);

                if (map_iter == new_vn_map.end())
                {
                    std::string s;
                    other = this->get_unuse_vn();
                    vn.insert(other);
                    
                    new_vn_map.insert(std::pair<char, VN_TYPE>(ch, other));
                    const std::string & str = save_find->second;
                    if (str.size() == 1)
                        this->add_production(other, "@");
                    else 
                        this->add_production(other, str.substr(1, str.size()));
                    
                    s += ch;
                    s += other;
                    this_production.insert(s);
                    this_production.erase(save_find->second);
                }
                else other = map_iter->second;

                if (iter->size() == 1)
                    this->add_production(other, "@");
                else 
                    this->add_production(other, iter->substr(1, iter->size()));
                
                iter = this_production.erase(iter);
            }
        }

        for (auto pair : new_vn_map)
            this->extract_one_left_divisor(pair.second);
    }
} // namespace Compiler
