//
// Created by qz915 on 2019/5/16.
//

#ifndef MYSHELL_BUILTINCOMMAND_HPP
#define MYSHELL_BUILTINCOMMAND_HPP

#include <string>
#include <vector>
#include <unistd.h>
#include <functional>
#include <map>
#include <utility>
#include <cstdlib>

#include "utilities.hpp"
#include "MyShell.hpp"

extern std::string host;
extern std::string user;
extern std::string path;
extern std::string relpath;
extern std::string userhome;

typedef std::map<std::string, std::string> Alias;
typedef std::function<int(char **, size_t)> func;

std::map<std::string, func> builtin_function;
Alias *als;

namespace builtin {
    int cd(char **, size_t);

    int pwd(char **, size_t);

    int exit(char **, size_t);

    int alias(char **, size_t);
}

int builtin::cd(char **argv, size_t argc) {
    if (argc > 2) {
        std::cout << "Shell: too many args" << std::endl;
        return -1;
    }
    std::string p;
    if (argv[1] == nullptr)
        p = userhome;
    else if (argv[1][0] == '~') {

        p += userhome;
        p += argv[1][1];
    } else {
        p = argv[1];
    }
    int ret = chdir(p.c_str());
    if (ret == 0) path = getcwd(NULL, 0);
    return ret;
}

int builtin::pwd(char **argv, size_t argc) {
    (void) argv;
    (void) argc;
    std::cout << path << std::endl;
    fflush(stdout);
    return 1;
}

void exit_shell()
{
    exit(1);
}

int builtin::exit(char **argv, size_t argc) {
    (void)argv;
    (void)argc;
    exit_shell();
    return -1;
}


int builtin::alias(char **argv, size_t argc) {
    std::string key;
    std::string raw;
    std::string str;
    int idx = 1;
    while(argv[idx]!=nullptr)
    {
        str+=argv[idx++];
        str+= ' ';
    }

    idx = 0;
    while(str[idx]!='=')
    {
        key+=str[idx++];
    }
    idx+=2;
    while(str[idx]!='"')
    {
        raw += str[idx++];
    }

    Alias &other = *als;
    other[key] = raw;

    return 0;
}

void InitBuiltinFunction(Alias &al) {
    als = &al;
    builtin_function.insert(std::make_pair<std::string, func>("cd", builtin::cd));
    builtin_function.insert(std::make_pair<std::string, func>("pwd", builtin::pwd));
    builtin_function.insert(std::make_pair<std::string, func>("exit", builtin::exit));
    builtin_function.insert(std::make_pair<std::string, func>("alias", builtin::alias));
}

#endif //MYSHELL_BUILTINCOMMAND_HPP
