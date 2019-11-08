#include <grammar.h>
#include <fstream>
#include <iostream>
#include <string>
#include <cstring>

using std::ifstream;
using std::cout;
using std::endl;
using std::string;

#define BUFFER_SIZE 128

int scan(const char *buffer, string & s, size_t *index);

bool iskeyword(const string & s);

const char *skipblank(const char *p);

inline bool isoct(const char c);

inline bool ishex(const char c);

inline void trim(string & s);

int main(int argc, char *argv[])
{
    if (argc != 2)
    {
        printf("Usage: %s filename\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    ifstream fin = ifstream(argv[1], std::ios_base::in);
    string token;
    string s;
    size_t index;

    if (!fin.is_open())
    {
        cout << "Can't open " << argv[1] << endl;
        exit(EXIT_FAILURE);
    }

    while (getline(fin, token))
    {
        index = 0;

        trim(token);
        if (token.empty()) continue;

        while (index < token.size())
        {
            int result = scan(token.c_str() + index, s, &index);
            if (result < 4)
            {
                cout << "<" << result << ", " << s << ">" << endl;
            }
            else if (result != -1)
            {
                cout << "<" << s << ", " << "->" << endl;
            } 
            else
            {
                cout << "ERROR::" << token << endl;
                exit(EXIT_FAILURE);
            }
        }
    }
}

int scan(const char *buffer, string & s, size_t *index)
{
    const char *p = buffer;
    int result;
    s.erase(s.cbegin(), s.cend());
    s += *p;

    if (isalpha(*p))
    {
        while (isalpha(*(++p)) || isdigit(*p))
            s += *p;

        if (iskeyword(s)) result = 4;
        else result = 0;
    }
    else if (*p == '0')
    {
        if (*(++p) == 'x' || *p == 'X')
        {
            s += *p;
            if (ishex(*++p))
            {
                s += *p;
                while (ishex(*(++p)))
                    s += *p;
                result = 3;
            }
            else result = -1;
        }
        else if (isdigit(*p))
        {
            if (*p != '0' && *p != '8' && *p != '9')
            {
                s += *p;
                while (isoct(*++p))
                    s += *p;
                result = 2;
            }
            else result = -1;
        }
        else result = 1;        
    }
    else if (isdigit(*p))
    {
        while (isdigit(*(++p)))
        {
            s += *p;
        }
        result = 1;
    }
    else ++p, result = 5;

    p = skipblank(p);

    *index += (p - buffer);
    return result;
}

void trim(string & s)
{
    if (!s.empty())
    {
        size_t i;
        for (i = 0; i < s.size() && s.at(i) < 33 ; ++i);
        if (i != 0) s.erase(0, i);

        if (!s.empty())
        {
            for (i = s.size() - 1; s.at(i) < 33; --i);
            if (i != s.size() - 1) s.erase(i + 1);
        }
    }
}

bool iskeyword(const string & s)
{
    if (s.at(0) == 'i')
        return s.compare("if") == 0;
    else if (s.at(0) == 't')
        return s.compare("then") == 0;
    else if (s.at(0) == 'd')
        return s.compare("do") == 0;
    else if (s.at(0) == 'e')
        return s.compare("eles") == 0;
    else if (s.at(0) == 's')
        return s.compare("where") == 0;
    

    switch (s.at(0))
    {
    case 'd':
        return s.compare("do") == 0;
    case 'e':
        return s.compare("else") == 0;
    case 'i':
        return s.compare("if") == 0;
    case 't':
        return s.compare("then") == 0;
    case 'w':
        return s.compare("where") == 0;   
    default:
        return false;
    }
}

const char *skipblank(const char *p)
{
    while (*p++ < 33);

    return p - 1;
}

bool isoct(const char c)
{
    return c < '8' && c > 47; // '0' 48
}

bool ishex(const char c)
{
    return (c < 58 && c > 47) || (c < 'g' && c > 96); // '0' 48 '9' 57 'a' 97
}


