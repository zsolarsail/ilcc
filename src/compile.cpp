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

#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <errno.h>
#include <dlfcn.h>

#include <sstream>
#include <fstream>

#include <libilcc/md5.h>

#include "misc.h"


#include "compile.h"

// ----------------------

bool std_h = true;
bool std_hpp = true;
bool lib_ilcc = true;

bool use_entry = true;

bool lib_no_delete = false;

// ----------------------

static const string entry_decl = "extern \"C\" int ilcc_entry(int argc, char **argv){";
static const string entry = "@entry";

// ----------------------

string build_name(const string &code)
{
    return ilcc::md5(code);
};

// ----------------------

bool mk_code(const string &src, string &code)
{
    stringstream f(ios::out);
    
    if(std_h) {
	f<< 
	    "#include <unistd.h>\n"
	    "#include <stdio.h>\n"
	    "#include <stdlib.h>\n"
	    "#include <string.h>\n"
	    "#include <fcntl.h>\n"
	    "#include <errno.h>\n"
	    "#include <sys/stat.h>\n"
	    "#include <sys/types.h>\n"
	    "\n";
    };

    if(std_hpp) {
	f<< 
	    "#include <string>\n"
	    "#include <memory>\n"
	    "#include <utility>\n"
	    "#include <vector>\n"
	    "#include <map>\n"
	    "#include <set>\n"
	    "#include <algorithm>\n"
	    "#include <iostream>\n"
	    "#include <fstream>\n"
	    "#include <sstream>\n"
	    "\n"
	    "using namespace std;\n"
	    "\n";
    };

    const char *p = src.c_str();
    const char *q = strstr(p, entry.c_str());

    if(q) {
    }
    else {
	f << entry_decl << endl;
	f << src << endl;
	f << "return 0;" << endl;
	f << "};" << endl;
    };


    code = f.str();
    return true;
};


// ----------------------
#pragma GCC diagnostic ignored "-Wunused-result"

bool compile(const string &code, const string &dest_name)
{
    FileCleaner cleaner;

    mk_wdir();
    string code_name = wdir_s + s_pid + "_code.cpp";
    cleaner.add(code_name);
    
    {
	fstream f(code_name, ios::out);
	if(!f)
	    return false;
	f << code;
    };
    
    string tmp_dest = wdir_s + s_pid + "-tmp.so";
    
    string cmd = "gcc -std=c++11 -O2 -pthread -fPIC --shared -o " + tmp_dest + " " + code_name;
    
    system(cmd.c_str());
    
    if(!file_exists(tmp_dest))
	return false;

    errno = 0;    
    int ret = ::rename(tmp_dest.c_str(), dest_name.c_str());
    if(ret) {
	int err = errno;
	fprintf(stderr, "%d: %s\n", err, strerror(err));
    };
    
    
    if(file_exists(dest_name))
	return true;
    
    return false;
};


// ----------------------

string build(const string &src)
{
    string name = build_name(src);
    string res = find_app(name);
    
    if(res.empty()) {
	string code;
	string app = app_name(name);
	
	if(mk_code(src, code) && compile(code, app))
	    res = app;
    };

    return res;
};


// ----------------------

typedef int (*prog_f)(int, char**);

string run(const string &app, int index, int argc, char **argv)
{
    char *a[argc - index + 2];
    a[0] = argv[0];
    int ac = 1;
    for(; index<argc; ++index)
	a[ac++] = argv[index];
    a[ac] = 0;
	
    int flags = RTLD_LAZY;
    if(lib_no_delete)
	flags |= RTLD_NODELETE;

    auto h = dlopen(app.c_str(), flags);
    if(!h)
	return string(dlerror());
    
    prog_f entry = (prog_f) dlsym(h, "ilcc_entry");
    if(!entry)
	return string(dlerror());

    entry(ac, a);
    dlclose(h);
    return string();
};


// ----------------------

