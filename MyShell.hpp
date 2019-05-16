#pragma once
#include <iostream>
#include <unistd.h>
#include <sys/wait.h>
#include <cstdio>
#include <cstring>
#include "utilities.hpp"
#include "builtincommand.hpp"

/*
void split(char *command, char **argv)
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

std::string host = "localhost";
std::string user = "user";
std::string path = "~";

//命令的最大长度
const size_t BUF_SIZE = 1024;
//最大参数个数
const size_t MAX_ARGS = 1024;

void MyShell()
{
    auto buf = new char[BUF_SIZE]();
    auto argsbuf = new char*[MAX_ARGS]();
    while (true)
    {
        // 1.打印提示符
        //std::cout << "[MyShell@localhost gerald]~ ";
        std::cout << '[' << user << '@' << host<<' '<< path << "]$";
        // 刷新缓冲区把这个提示符打出来
        fflush(stdout);

        // 2.接收产出
        char *command_buf = buf;
        //gets(command);
        size_t length = utils::GetLine(command_buf, BUF_SIZE);

        // 3.分割命令行参数
        char **argv = argsbuf;
        size_t args = utils::Split(command_buf, argv, length);

        // 4.execv
        int exit_code = DoExecv(argv);
        if(exit_code == -1)
            break;
    }
    delete[] argsbuf;
    delete[] buf;
}
