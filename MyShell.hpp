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

// 创建子进程，并让子进程执行刚才输入的命令
int DoExecv(char *argv[])
{
    // 创建子进程
    pid_t id = fork();
    if (id < 0)
        perror("fork");

    if (id == 0)// Child
    {
       execvp(argv[0], argv);
    }
    
    int status = 0;
    wait(&status);
    
    if ((status & 0xff) == 0) // 正常退出
    {
        status >>= 8;
        return (status & 0xff);
    }

    std::cout << "Abnormal Exit ! The signal is :" << (status & 0x7f) << std::endl;
    return -1;
}

int Parse(utils::argument *args)
{
    char **arg = new char*[64];
    int i=0;
    for(auto it = args->argsvec[0].begin(); it!=args->argsvec[0].end(); ++it)
    {
        arg[i++] = *it;
    }

    if(builtin_function.count(arg[0]))
    {
        return builtin_function[arg[0]](arg, args->argsvec.size());
    }
    else
    {
        return DoExecv(arg);
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
        utils::argument* argv = utils::Split(command_buf, length, alias);

        int exit_code = Parse(argv);

        if(exit_code == -1)
            break;
    }

    delete[] argsbuf;
    delete[] buf;
}
