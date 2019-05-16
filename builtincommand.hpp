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

#include "utilities.hpp"

typedef std::map<std::string, std::string> Alias;
typedef std::map<std::string, std::string>::iterator AliasIter;
typedef std::function<int(char**, size_t)> func;

std::map<std::string, func> builtin_function;

namespace builtin{
    int cd(char **, size_t);
    int pwd(char **, size_t);
}

int builtin::cd(char **argv, size_t args)
{
    if(args > 2 )
    {
        std::cout << "Shell: too many args" << std::endl;
        return -1;
    }
    int ret;
    std::string fullPath;
    if(args == 1)  //没有参数
    {
        fullPath = "/home/";
        fullPath += user;
    }
    else if(argv[1][0] == '~')  //参数是用户的家目录
    {
        fullPath = "/home/";
        fullPath += user;
        fullPath += argv[1][1];
    }
    else if(argv[1][0] == '/')   //绝对路径
    {
        fullPath = argv[1];
    } else{                   //从当前路径开始
        fullPath = path;
        fullPath += '/';
        fullPath += argv[1];
    }
    ret = chdir(fullPath.c_str());
    if(ret == 0)
        path = fullPath;
    return ret;
}

int builtin::pwd(char **argv, size_t args)
{
    (void)argv;
    (void)args;
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
