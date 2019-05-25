//
// Created by qz915 on 2019/5/16.
//

#ifndef MYSHELL_UTILITIES_HPP
#define MYSHELL_UTILITIES_HPP

#include <sstream>
#include <iostream>
#include <sys/types.h>
#include <pwd.h>
#include <unistd.h>
#include <map>
#include <sys/fcntl.h>
#include <cstring>

extern std::string host;
extern std::string user;
extern std::string path;
extern std::string relpath;
extern std::string userhome;
extern void InitBuiltinFunction();

extern const size_t BUF_SIZE;
extern const size_t MAX_ARGS;

// 维护一个键值对，用来定义别名
typedef std::map<std::string, std::string> Alias;
typedef std::map<std::string, std::string>::iterator AliasIter;

namespace utils{
    size_t GetLine(char *, size_t);
    int Split(char *, char **, size_t);
    void Init(Alias& alias);
    void DoRedirect(char *command);
    int sfd;
}

void MakeDefualtAlias(Alias& alias)
{
    // 在这里添加一启动就自定义的别名
    alias["ll"] = "ls -l";
    alias["rm"] = "rm -i";
    alias["cp"] = "cp -i";
    alias["mv"] = "mv -i";
    alias["l"] = "ls -a";
}

void utils::Init(Alias& alias)
{
    // 获取用户名
    struct passwd *user_info;
    user_info = getpwuid(getuid());
    user = user_info->pw_name;

    // 获取当前绝对路径
    char *buf = new char[BUF_SIZE];
    path = getcwd(buf, BUF_SIZE);
    delete[] buf;

    // 获得当前主机名
    buf = new char[BUF_SIZE];
    gethostname(buf, BUF_SIZE);
    host = buf;
    delete[] buf;

    //设置用户目录
    userhome += "/home/";
    userhome += user;

    // 生成一个默认的键值对
    MakeDefualtAlias(alias);
    InitBuiltinFunction(); 
}

size_t utils::GetLine(char *buf, size_t bufsize)
{
    int count =0;
    char c=0;
    while(count < static_cast<int>(bufsize))
    {
        c = static_cast<char>(std::cin.get());
        if(c == '\n')
            break;
        else
        {
            if(c == '\\')
            {
                std::cin.get();
                std::cout << '>';
            }
            else
            {
                buf[count++] = c;
            }
        }
    }
    buf[count] = '\0';
    return count;
}

int utils::Split(char *command, char **argv, size_t cmdlength)
{
    utils::DoRedirect(command);// 现在处理重定向或追加
    size_t idx = 0;
    int cnt = 0;     //参数个数
    while(idx < cmdlength && command[idx] != '\0')
    {
        //处理空格
        while(command[idx] == ' ')
            ++idx;

        argv[cnt++] = command+idx;//参数的开始
        while(idx < cmdlength && command[idx] != ' ' && command[idx] != '\0')
            ++idx;
        command[idx++] = 0;  //参数结尾补0
    }
    argv[cnt] = nullptr;//参数数组结尾nullptr
    return cnt;
}

void utils::DoRedirect(char *command)
{
    char *file = nullptr;
    char *ptr = command;
    int type_redirect = -1;
    int fd;
    utils::sfd = dup(1);

    while (*ptr != '\0')
    {
        if (*ptr == '>') //判断是否是重定向还是追加
        {
            *ptr++ = '\0';
            type_redirect++;

            if (*ptr == '>')
            {
                *ptr++ = '\0';
                type_redirect++;
            }

            while (*ptr == ' ')
            {
                ptr++;
            }
            // 复制文件名
            file = ptr;
            while (*ptr != ' ' && *ptr != '\0')
            {
                ptr++;
            }
            *ptr = '\0';

            // 根据重定向或者追加来以不同的属性来创建文件
            if (type_redirect == 0)
            {
                fd = open(file, O_CREAT | O_TRUNC | O_WRONLY, 0664);
            }
            else if (type_redirect == 1)
            {
                fd = open(file, O_CREAT | O_APPEND | O_WRONLY, 0664);
            }

            dup2(fd, 1);
        }

        ptr++;
    }
}

#endif //MYSHELL_UTILITIES_HPP
