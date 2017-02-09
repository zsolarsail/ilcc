#pragma once

/*
MIT License

Copyright zsolarsail(c) 2017 <jwretsam@gmail.com>

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
*/


#include <string>

namespace ilcc {

using namespace std;


// ----------------------
void md5(void * hash, const void * in_buf, unsigned int buf_len);

inline void md5(void * hash, const std::string & in)
{
    md5(hash, in.c_str(), in.length());
};

// ----------------------

union md5_hash_t
{
    unsigned char c[16];
    uint32_t l[4];

    void erase(void)
    {
	l[0] = l[1] = l[2] = l[3] = 0;
    };


    bool empty(void) const
    {
	return !l[0] && !l[1] && !l[2] && !l[3];
    };

};


inline bool md5_hash_equ(const md5_hash_t & a, const md5_hash_t & b)
{
    return      a.l[0] == b.l[0] &&
                a.l[1] == b.l[1] && 
                a.l[2] == b.l[2] &&
                a.l[3] == b.l[3];
};


inline bool md5_hash_lt(const md5_hash_t & a, const md5_hash_t & b)
{
    if(a.l[0] < b.l[0])
        return true;
    if(a.l[0] > b.l[0])
        return false;
    
    if(a.l[1] < b.l[1])
        return true;
    if(a.l[1] > b.l[1])
        return false;
    
    if(a.l[2] < b.l[2])
        return true;
    if(a.l[2] > b.l[2])
        return false;
    
    return a.l[3] < b.l[3];
};


inline bool operator == (const md5_hash_t & a, const md5_hash_t & b) { return md5_hash_equ(a, b); };
inline bool operator <  (const md5_hash_t & a, const md5_hash_t & b) { return md5_hash_lt(a, b); };

// ----------------------

struct md5_t
{
    md5_hash_t hash;
    
    md5_t() { erase(); };
    md5_t(const md5_t & x) : hash(x.hash) { };
    md5_t(const void *data, unsigned int len) { set(data, len); };
    md5_t(const string & str) { set(str); };

    md5_t & operator = (const string & str) { set(str); return *this; };

    bool assign(const char *md5_str);
    bool assign(const string &md5_str);

    void erase(void) { hash.erase(); };
    
    void set(const void *data, unsigned int len) { md5(hash.c, data, len); };
    void set(const string & str) { md5(hash.c, str); };

    int sprint(char *buf) const;
    
    string as_string(void) const
    {
	char buf[64];
	sprint(buf);
	return string(buf, 32);
    };

    bool empty(void) const { return hash.empty(); };
};

inline bool operator == (const md5_t & a, const md5_t & b) { return a.hash == b.hash; };
inline bool operator <  (const md5_t & a, const md5_t & b) { return a.hash <  b.hash; };


inline string md5(const string &s)
{
    md5_t m;
    m = s;
    return m.as_string();
};


}; // ilcc
