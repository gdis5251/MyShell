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
#include <vector>
#include <list>
#include <regex>

extern std::string host;
extern std::string user;
extern std::string path;
extern std::string relpath;
extern std::string userhome;

extern void InitBuiltinFunction();

extern const size_t BUF_SIZE;
extern const size_t MAX_ARGS;

const int NO_ALIAS = -123;

// 维护一个键值对，用来定义别名
typedef std::map<std::string, std::string> Alias;
typedef std::map<std::string, std::string>::iterator AliasIter;

namespace utils {
    struct argument;

    size_t GetLine(char *, size_t);

    argument *Split(char *, size_t, Alias &);

    void Init(Alias &alias);

    int ReplaceAlias(argument *args, Alias &alias);

    inline void SkipSpace(char *ps, int &idx) {
        if (*ps != 0 && *ps == ' ')
            ++idx;
    }

    utils::argument *Split(char *cmdbuf, size_t length, Alias &alias);

    void Init(Alias &alias);

    void DoRedirect(char *command);

    int sfd;
}

struct utils::argument {
    argument() {//别名缓冲区
        aliasbuf = new char *[16];
        redirecFile = nullptr;
        for (int i = 0; i < 16; ++i) {
            aliasbuf[i] = new char[64];
        }
    }

    char *redirecFile; //重定向的目标文件
    std::vector<std::list<char *>> argsvec;
    char **aliasbuf;
    //这个vector保存 参数列表 用于管道
    //里面的list保存的就是参数的字符串
};

void MakeDefualtAlias(Alias &alias) {
    // 在这里添加一启动就自定义的别名
    alias["ll"] = "ls -l";
    alias["rm"] = "rm -i";
    alias["cp"] = "cp -i";
    alias["mv"] = "mv -i";
    alias["l"] = "ls -a";
}

void utils::Init(Alias &alias) {
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

size_t utils::GetLine(char *buf, size_t bufsize) {
    size_t count = 0;
    char c = 0;
    while (count < bufsize) {
        c = static_cast<char>(std::cin.get());
        if (c == '\n')
            break;
        else {
            if (c == '\\') {
                std::cin.get();
                std::cout << '>';
            } else {
                buf[count++] = c;
            }
        }
    }
    buf[count] = '\0';
    return count;
}

utils::argument *utils::Split(char *cmdbuf, size_t length, Alias &alias) {
    using namespace utils;

    argument *arg = new argument();
    int idx = 0;

    while (idx < length) {
        std::list<char *> vec;   //管道的一端
        SkipSpace(cmdbuf, idx);
        while (cmdbuf[idx] != 0 && cmdbuf[idx] != '|') {     //每段管道都添加到argsvec中
            // 如果没有管道argsvec就只有一个元素
            if (cmdbuf[idx] == '>')//重定向到文件名  并跳过这个文件名
            {
                ++idx;//跳过 '>'
                SkipSpace(cmdbuf, idx);

                arg->redirecFile = cmdbuf + idx;   //文件名

                while (cmdbuf[idx] != ' ') ++idx;

                cmdbuf[idx++] = 0;            //文件名后加nullptr
            } else {//正常的命令参数
                vec.push_back(cmdbuf + idx);

                while (cmdbuf[idx] != 0 && cmdbuf[idx] != ' ')
                    ++idx;
                cmdbuf[idx++] = 0;
            }
        }
        arg->argsvec.push_back(vec);
    }
    ReplaceAlias(arg, alias);
    return arg;
}

int utils::ReplaceAlias(utils::argument *args, Alias &alias) {
    int i = 0;
    for (auto lst = args->argsvec.begin(); lst != args->argsvec.end(); ++lst, ++i) {
        std::string name(lst->front());
        int cnt = alias.count(name);
        if (cnt < 1)    //没有别名
            return NO_ALIAS;
        else {
            std::string str = alias[name];
            lst->pop_front();
            char *buf = args->aliasbuf[i];
            const char *tmp = str.c_str();
            int x;
            for (x = 0; x < str.size(); ++x) {
                buf[x] = tmp[x];
            }
            buf[x] = 0;
            std::list<char *> l;
            int idx = 0;
            while (idx < str.size()) {
                utils::SkipSpace(buf, idx);
                l.push_back(buf + idx);
                while (buf[idx] != 0 && buf[idx] != ' ') ++idx;
                buf[idx++] = 0;
            }

            for (auto i = l.rbegin(); i != l.rend(); ++i)
                lst->push_front(*i);
        }
    }
    return 1;
}

void utils::DoRedirect(char *command) {
    char *file = nullptr;
    char *ptr = command;
    int type_redirect = -1;
    int fd;
    utils::sfd = dup(1);

    while (*ptr != '\0') {
        if (*ptr == '>') //判断是否是重定向还是追加
        {
            *ptr++ = '\0';
            type_redirect++;

            if (*ptr == '>') {
                *ptr++ = '\0';
                type_redirect++;
            }

            while (*ptr == ' ') {
                ptr++;
            }
            // 复制文件名
            file = ptr;
            while (*ptr != ' ' && *ptr != '\0') {
                ptr++;
            }
            *ptr = '\0';

            // 根据重定向或者追加来以不同的属性来创建文件
            if (type_redirect == 0) {
                fd = open(file, O_CREAT | O_TRUNC | O_WRONLY, 0664);
            } else if (type_redirect == 1) {
                fd = open(file, O_CREAT | O_APPEND | O_WRONLY, 0664);
            }

            dup2(fd, 1);
        }

        ptr++;
    }
}


//把解析字符串封装到类中
class Command{
public:
    Command(char *str)
    : cmdline(str), redirect()
    {
		MetchRedirect();
		Split();
    }

private:
    std::string cmdline;
    std::vector<std::list<std::string>> cmd;
    std::string redirect;
private:
    void Split()
    {
        std::regex pip("\\s+[|]\\s+");
        std::regex space("\\s+");
        std::sregex_token_iterator pipbeg(cmdline.begin(), cmdline.end(), pip, -1);
        std::sregex_token_iterator end;
        while(pipbeg!=end)  //分割管道
        {
            std::string arg(*pipbeg);
            std::sregex_token_iterator argbeg(arg.begin(), arg.end(), space, -1);  //分割参数
            std::list<std::string> lst;
            cmd.push_back(lst);
            while(argbeg!=end)
            {
                lst.push_back(*argbeg);
                ++argbeg;
            }
            ++pipbeg;
        }
    }

    void MetchRedirect()
    {
        std::smatch res1;
        std::smatch res2;
        std::regex rd(" [>12]*> [a-zA-Z.]+ *");
        std::regex redir("[a-zA-Z.]+");
        if(std::regex_search(cmdline, res1, rd))  //匹配到了重定向
        {
            std::string str(res1[0]);
            std::regex_search(str, res2, redir);
            redirect = res2[0];
            cmdline = regex_replace(cmdline, rd, "");
        }
    }
};

#endif //MYSHELL_UTILITIES_HPP
