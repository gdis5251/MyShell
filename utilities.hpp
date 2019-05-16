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

extern std::string host;
extern std::string user;
extern std::string path;

// 缓冲区的最大长度
const size_t BUF_SIZE = 1024;
// 最大参数个数
const size_t MAX_ATGS = 1024;

namespace utils{
    size_t getline(char *, size_t);
    void changedir(const std::string&);
    void Init(void);
    
}

void utils::Init(void)
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

}

size_t utils::getline(char *buf, size_t bufsize)
{
    int count =0;
    char c=0;
    while(count < bufsize)
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

void utils::changedir(const std::string & dir)
{
    //TODO: change dir
}

#endif //MYSHELL_UTILITIES_HPP
