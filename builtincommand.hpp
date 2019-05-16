//
// Created by qz915 on 2019/5/16.
//

#ifndef MYSHELL_BUILTINCOMMAND_HPP
#define MYSHELL_BUILTINCOMMAND_HPP
#include <string>
#include <vector>
#include "utilities.hpp"

extern std::string host;
extern std::string user;
extern std::string path;

namespace builtin{
    int cd(const std::string&);
    std::string& pwd();
}

int builtin::cd(const std::string& targetPath)
{
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

std::string& builtin::pwd()
{
    return path;
}

#endif //MYSHELL_BUILTINCOMMAND_HPP
