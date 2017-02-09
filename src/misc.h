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
#include <vector>

using namespace std;

// ----------------------

extern long uid;
extern string s_uid;

extern string wdir;
extern string wdir_s;

extern string s_pid;
// ----------------------

void mk_root(void);
void mk_wdir(void);

// ----------------------

bool file_exists(const string &path);

// ----------------------

string app_name(const string &name);
string find_app(const string &name);

// ----------------------

class FileCleaner {
    vector<string> files;

public:
    FileCleaner() = default;
    ~FileCleaner() { remove(); };
    void add(const string &name) { files.push_back(name); };
    void remove(void);
};


// ----------------------

