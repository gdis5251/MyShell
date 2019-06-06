#pragma once
#include <iostream>
#include <unistd.h>
#include <sys/wait.h>
#include <cstdio>
#include <sys/types.h>
#include <pwd.h>
#include <cstring>

#include "utilities.hpp"
#include "builtincommand.hpp"

//命令的最大长度
const size_t BUF_SIZE = 1024;
//最大参数个数
const size_t MAX_ARGS = 1024;

std::string host = "localhost";
std::string user = "user";
std::string path = "~";
std::string relpath = "";
std::string userhome = "";

extern std::map<std::string, func> builtin_function;

int Execute(Command &cmd)
{
    /*
    if(!cmd.isExec())
    {
        return 0;
    }
     */
    char **arg1 = cmd.GetArgv(0);
    int length = cmd.GetArgLen(0);
    if(cmd.HasRediredt())   //重定向
    {
        int old = dup(STDOUT_FILENO);
        close(STDOUT_FILENO);
        const char *file = cmd.GetRedir();
        int flag = cmd.GetRedirectArg();
        int fd = open(file, flag, 0664);
        int ret = utils::exe(arg1[0], arg1, length);

        close(fd);
        dup(old);
        close(old);
        return ret;
    }
    else if(cmd.HasPipe())    //管道
    {
        return -1;
    }
    else{     //单行命令
        return utils::exe(arg1[0], arg1, length);
    }
}

void RelPath()
{
    if(path.substr(0,6) == userhome.substr(0,6))
    {
        relpath = ("~" + path.substr(6+user.size()));
    } else{
        relpath = path;
    }
}

void MyShell()
{

    auto buf = new char[BUF_SIZE]();
    auto argsbuf = new char*[MAX_ARGS]();
    
    // 初始化MyShll
    Alias alias;
    utils::Init(alias);

    while (true)
    {
        // 1.打印提示符
        RelPath();
        std::cout << '[' << user << '@' << host<<' '<< relpath << "]$ ";

        // 刷新缓冲区把这个提示符打出来
        fflush(stdout);

        // 2.接收产出  length  参数个数
        char *command_buf = buf;
        size_t length = utils::GetLine(command_buf, BUF_SIZE);
        if(length == 0)
            continue;
        // 3.分割命令行参数
        Command cmd(command_buf,alias);
        int exit_code = Execute(cmd);

        if(exit_code == -1)
            break;
    }

    delete[] argsbuf;
    delete[] buf;
}
