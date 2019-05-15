#pragma once
#include <iostream>
#include <unistd.h>
#include <sys/wait.h>
#include <cstdio>
#include <string.h>


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

void MyShell(void)
{
    while (1)
    {
        // 1.打印提示符
        std::cout << "[MyShell@localhost gerald]~ ";
        // 刷新缓冲区把这个提示符打出来
        fflush(stdout);

        // 2.接收产出
        char command[1024] = {0};
        gets(command);

        // 3.解析argv
        char *argv[1024] = {0};
        split(command, argv);

        // 4.execv
        int exit_code = DoExecv(argv);

    }
}
