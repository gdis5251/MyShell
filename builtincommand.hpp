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

void cd(const std::string& targetPath)
{
    std::string realPath;
    if(targetPath[0] !='~')
    {
        realPath = targetPath;
        path = realPath;
    }
    else
    {
        realPath+="/home/";
        realPath+=user;
        realPath+='/';
        realPath+=targetPath.substr(1);
        path = targetPath;
    }
    utils::changedir(realPath);
}

#endif //MYSHELL_BUILTINCOMMAND_HPP
