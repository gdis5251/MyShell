#pragma once
#include <iostream>
#include <unistd.h>
#include <sys/wait.h>
#include <cstdio>
#include <sys/types.h>
#include <pwd.h>
#include <string.h>
#include <cstring>
#include "utilities.hpp"
#include "builtincommand.hpp"

/*
void split(char *command, char **argv)
#include <map>

// 维护一个键值对，用来定义别名
typedef std::map<std::string, std::string> Alias;
typedef std::map<std::string, std::string>::iterator AliasIter;



// 将输入进来的命令根据空格拆分开
void split(char *command, char *argv[])

{
    char *p = strtok(command, " ");
    int i = 0;
    
    while (p != nullptr)
    {
        argv[i++] = p;
        p = strtok(nullptr, " ");
    }
    argv[i] = nullptr;
}
 */
std::string host = "localhost";
std::string user = "user";
std::string path = "~";

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

int Parse(char **argv, size_t args)
{
    std::string f(argv[0]);
    if(builtin_function.count(f))
    {
        return builtin_function[f](argv, args);
    }
    else
    {
        return DoExecv(argv);
    }
}


//命令的最大长度
const size_t BUF_SIZE = 1024;
//最大参数个数
const size_t MAX_ARGS = 1024;


void MyShell()
{
    InitBuiltinFunction();

    auto buf = new char[BUF_SIZE]();
    auto argsbuf = new char*[MAX_ARGS]();
    auto argsbuf = new char*[MAX_ATGS]();
    
    // 初始化MyShll
    Alias alias;
    utils::Init(alias);



    while (true)
    {
        // 1.打印提示符
        
        //std::cout << "[MyShell@localhost gerald]~ ";
        std::cout << '[' << user << '@' << host<<' '<< path << "]$";

        // 刷新缓冲区把这个提示符打出来
        fflush(stdout);

        // 2.接收产出
        char *command_buf = buf;
        size_t length = utils::GetLine(command_buf, BUF_SIZE);

        // 3.分割命令行参数
        char **argv = argsbuf;
        size_t args = utils::Split(command_buf, argv, length);

        int exit_code = Parse(argv, args);
        // 4.execv
        //int exit_code = DoExecv(argv);
        if(exit_code == -1)
            break;
    }

    delete[] argsbuf;
    delete[] buf;
}
