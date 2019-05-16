//
// Created by qz915 on 2019/5/16.
//

#ifndef MYSHELL_UTILITIES_HPP
#define MYSHELL_UTILITIES_HPP

#include <sstream>
#include <iostream>

namespace utils{
    size_t GetLine(char *, size_t);
    int Split(char *, char **, size_t);
}

size_t utils::GetLine(char *buf, size_t bufsize)
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

int utils::Split(char *command, char **argv, size_t cmdlength)
{
    int idx = 0;
    int cnt=0;     //参数个数
    while(idx<cmdlength)
    {
        //处理空格
        while(command[idx] == ' ')
            ++idx;

        argv[cnt++] = command+idx;//参数的开始
        while(command[idx] != ' ')
            ++idx;
        command[idx++] = 0;  //参数结尾补0
    }
    argv[cnt] = 0;//参数数组结尾nullptr
    return cnt;
}

#endif //MYSHELL_UTILITIES_HPP
