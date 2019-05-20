# MyShell

## 开发人员：

郭文峰  [GitHub](<https://github.com/gdis5251>)

秦哲  [GitHub](<https://github.com/Qzhe>)

## 项目简介：

我们尝试自己写一个shell，期望在bash的基础上，添加更多好玩的功能。



## 新手村版本：

就是一个很简陋的Shell，不支持内建命令，提示符都是静态写死的一个字符串。



## 装备升级区：

- 动态获取 **用户名、主机名、绝对路径**。

  - 获取用户名

    ```cpp
        // 获取用户名
        struct passwd *user_info;
        user_info = getpwuid(getuid());
        user = user_info->pw_name;
    ```

           The passwd structure is defined in <pwd.h> as follows:
        
               struct passwd {
                   char   *pw_name;       /* username */
                   char   *pw_passwd;     /* user password */
                   uid_t   pw_uid;        /* user ID */
                   gid_t   pw_gid;        /* group ID */
                   char   *pw_gecos;      /* user information */
                   char   *pw_dir;        /* home directory */
                   char   *pw_shell;      /* shell program */
               };
    `passwd`结构体是在<pwd.h>这个头文件里包含着，内涵元素在上面都有。

    `getpwuid()`是一个返回一个指向`passwd`结构体的指针。它的参数是是一个`uid`，可以用`getuid()`系统调用来获取到。

  - 获取绝对路径

    ```cpp
        // 获取当前绝对路径
        char *buf = new char[BUF_SIZE];
        path = getcwd(buf, BUF_SIZE);
        delete[] buf;
    ```

    `getcwd()`是获取当前文件的路径的一个系统调用。

  - 获取主机名

    ```cpp
        // 获得当前主机名
        buf = new char[BUF_SIZE];
        gethostname(buf, BUF_SIZE);
        host = buf;
        delete[] buf;
    ```

    `gethostname()`是获取当前主机名的系统调用。
  
- 支持内建命令 在得到命令行后解析判断是内建命令还是外部可执行程序

  - 用c++标准库的map 和 functional 映射函数名和函数调用
  - cd 没有参数切换到当前登录用户的家目录下
    第一个参数是 / 绝对路径 没有/为相对路径
    ```
    int chdir(char *path);
    切换到绝对路径或相对路径
    ~是shell的变量指向当前登录用户的家目录
    cd后不加参数为切换到用户家目录
    ```
  - pwd显示当前所在目录的绝对路径
  - shell输出的提示是如果在当前登录用户的家目录下输出~/...
    否则输出绝对路径
  
- 解决XShell 点击Backspace 出现 ^H 的问题。

  - ![在这里插入图片描述](https://img-blog.csdnimg.cn/20190517180018298.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dlaXhpbl80MjY3ODUwNw==,size_16,color_FFFFFF,t_70)
  - ![在这里插入图片描述](https://img-blog.csdnimg.cn/20190517180050375.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dlaXhpbl80MjY3ODUwNw==,size_16,color_FFFFFF,t_70)

