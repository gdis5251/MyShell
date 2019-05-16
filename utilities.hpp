//
// Created by qz915 on 2019/5/16.
//

#ifndef MYSHELL_UTILITIES_HPP
#define MYSHELL_UTILITIES_HPP

#include <sstream>
#include <iostream>

namespace utils{
    size_t getline(char *, size_t);
    void changedir(const std::string&);
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
