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

#include "md5imp.h"

#include "md5.h"

namespace ilcc {

void md5(void *hash, const void *in_buf, unsigned int buf_len)
{
    MD5((unsigned char*)in_buf, buf_len, (unsigned char *)hash);
};

// ----------------------

static const char *hexchar_table = "0123456789abcdef";

inline char byte2hex_l(uint8_t b) { return hexchar_table[b & 0xF]; };
inline char byte2hex_h(uint8_t b) { return hexchar_table[b >> 4]; };

static char * byte2hex(char *str, uint8_t b)
{
    *str++ = byte2hex_h(b);
    *str++ = byte2hex_l(b);
    return str;
};


static uint8_t hex2byte_c(char c)
{
    if(c >= '0' && c <= '9')
        return c - '0';

    if(c >= 'A' && c <= 'F')
        return c - 'A' + 10;

    if(c >= 'a' && c <= 'f')
        return c - 'a' + 10;

    return 0xFF;
};


static bool hex2byte(const char * str, uint8_t & val)
{
    val = hex2byte_c(str[0]);
    if(val > 0xF)
        return false;

    val <<= 4;

    uint8_t x = hex2byte_c(str[1]);
    if(x > 0xF)
        return false;

    val |= x;
    return true;
};

// ----------------------


int md5_t::sprint(char *buf) const
{
    char *s = buf;
    
    for(uint8_t x : hash.c)
	s = byte2hex(s, x);

    *s = 0;
    return 32;
};



// ----------------------

bool md5_t::assign(const char *md5_str)
{
    for(uint8_t &x : hash.c)
    {
	if(!hex2byte(md5_str, x))
	{
	    erase();
	    return false;
	};
	md5_str += 2;
    };	
    return true;
};


bool md5_t::assign(const string &md5_str)
{
    if(md5_str.length() == 32)
	return assign(md5_str.c_str());

    erase();
    return false;
};

}; // ilcc


