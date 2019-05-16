//
// Created by qz915 on 2019/5/16.
//

#ifndef MYSHELL_BUILTINCOMMAND_HPP
#define MYSHELL_BUILTINCOMMAND_HPP

#include <string>
#include <vector>
#include "utilities.hpp"
#include <unistd.h>
#include <functional>
#include <map>
#include <utility>

extern std::string host;
extern std::string user;
extern std::string path;

typedef std::function<int(char**, size_t)> func;

std::map<std::string, func> builtin_function;

namespace builtin{
    int cd(char **, size_t);
    int pwd(char **, size_t);
}

int builtin::cd(char **argv, size_t args)
{
    if(args>2)
    {
        // to many args
        return -1;
    }
    std::string targetPath(argv[1]);
    std::string realPath;
    if(targetPath[0] != '~')
    {
        realPath = targetPath;
        path = realPath;
    }
    else
    {
        realPath += "/home/";
        realPath += user;
        realPath += '/';
        realPath += targetPath.substr(1);
        path = targetPath;
    }
    return chdir(realPath.c_str());
}

int builtin::pwd(char **argv, size_t args)
{
    std::cout << path << std::endl;
    fflush(stdout);
    return 1;
}

void InitBuiltinFunction()
{
    builtin_function.insert(std::make_pair<std::string, func>("cd", builtin::cd));
    builtin_function.insert(std::make_pair<std::string, func>("pwd", builtin::pwd));

}

#endif //MYSHELL_BUILTINCOMMAND_HPP
