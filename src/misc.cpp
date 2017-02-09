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
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>

#include <sys/stat.h>
#include <sys/types.h>

#include <memory>

#include "misc.h"

// ----------------------

string root = "/tmp/ilcc";

long uid = 0;
string s_uid;

string wdir;
string wdir_s;

string s_pid;

// ----------------------

void mk_root(void)
{
    mkdir(root.c_str(), 0777);
};

void mk_wdir(void)
{
    mk_root();
    mkdir(wdir.c_str(), 0755);
};

// ----------------------

bool file_exists(const string &path)
{
    struct stat b;
    return (stat(path.c_str(), &b) == 0);
};

// ----------------------

string app_name(const string &name)
{
    return wdir_s + name;
};

// ----------------------

string find_app(const string &name)
{
    string w_name = app_name(name);
    string s;
    int ret;
    
    if(file_exists(w_name))
	return w_name;

    struct DirEntry {
	DIR *d;
	DirEntry(const string &dir) : d(opendir(dir.c_str())) { };
	~DirEntry() { if(d) closedir(d); };
    };
    
    DirEntry ed(root);
    
    if(ed.d) {
	struct dirent* p;
	
	while ((p = readdir(ed.d))) {
	    if (p->d_name[0] == '.' && (p->d_name[1] == 0 || (p->d_name[1] == '.' && p->d_name[2] == 0)))
		continue;
	
	    s = root + '/' + p->d_name + '/' + name;
	    
	    if(file_exists(s)) {
		ret = ::link(s.c_str(), w_name.c_str());
		if(!ret || file_exists(w_name))
		    return w_name;
	    };
	};
    };
    
    return string();
};

// ----------------------

void FileCleaner::remove(void)
{
    for(auto &x: files)
	::unlink(x.c_str());
	
    files.clear();
};


// ----------------------

namespace {

struct MiscInitializer {

    MiscInitializer() {
	s_pid = to_string(getpid());
	uid = getuid();
	s_uid = to_string(uid);
	wdir = root + '/' + s_uid;
	wdir_s = wdir + '/';
    };

};

MiscInitializer misc_initializer;

}; //
