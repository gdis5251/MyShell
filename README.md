# MyShell

## 开发人员：

郭文峰  [GitHub](https://github.com/gdis5251)

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