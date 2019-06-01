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
extern int utils::sfd;

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

    utils::sfd = dup2(utils::sfd, 1);

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

int Parse(Command &cmd)
{
    char **arg1 = cmd.GetArgv(0);
    if(cmd.HasRediredt())   //重定向
    {
        int old = dup(STDOUT_FILENO);
        close(STDOUT_FILENO);
        const char *file = cmd.GetRedir();
        int fd = open(file, );
    }
    else if(cmd.HasPipe())    //管道
    {

    }
    else{     //单行命令
        utils::exe(arg1[0], arg1);
    }

    /*
    if(builtin_function.count(arg[0]))
    {
        return builtin_function[arg[0]](arg, args->argsvec.size());
    }
    else
    {
        return DoExecv(arg);
        // 判断是否是 alias 定义的别名
        if (alias.count(f))
        { 
            f = alias[f.c_str()];
            std::string tmp_str;
            tmp_str = f.c_str();

            // 重新拆分字符串 
            char *tmp_argv[BUF_SIZE] = {0};
            int num = utils::Split(const_cast<char *>(tmp_str.c_str()), tmp_argv, f.size());  

            argv[0] = tmp_argv[0];
            // 将后续的参数补到 argv 里
            int i = 0;
            for (i = 1; i < num; i++)
            {
                argv[args + i - 1] = tmp_argv[i];
            }
            argv[args + i - 1] = 0;

            // 更新 args
            args += i - 1;
        }

        // ls 的系列命令需要加颜色
        if (strcmp(argv[0], "ls") == 0)
        {
            argv[args] = const_cast<char*>("--color=auto");
            argv[args + 1] = 0;
        }

        return DoExecv(argv);
    }
     */
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
        //utils::argument* argv = utils::Split(command_buf, length, alias);
        Command cmd(command_buf,alias);

        int exit_code = Parse(cmd);

        if(exit_code == -1)
            break;
    }

    delete[] argsbuf;
    delete[] buf;
}
