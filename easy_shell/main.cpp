#include <iostream>
#include <algorithm>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include "MyShell.h"

using namespace std;

/*
实现步骤： 
1. 接收用户输入命令字符串，拆分命令及参数存储。（自行设计数据存储结构）
2. 实现普通命令加载功能 
3. 实现输入、输出重定向的功能 
4. 实现管道 
5. 支持多重管道
*/

void myPrint(string str)
{
    cout << str << " ";
}

void printError(char *c)
{
    perror(c);
    exit(0);
}

int main()
{
    MyShell sh;
    while (true)
    {
        // 获取用户输入命令并进行拆分，格式为string
        sh.recInput();

        // 创建子进程
        sh.m_pid = fork();
        if (sh.m_pid < 0)
        {
            sh.printError("fork error");
        }
        else if (sh.m_pid != 0)
        {
            // 父进程负责打印用户输入的命令，并阻塞式等待子进程的结束
            cout << "output -------------------" << endl;
            for_each(sh.m_cmd.cmd.begin(), sh.m_cmd.cmd.end(), myPrint);
            cout << endl;

            sh.myWait(sh.m_pid);
        }
        else
        {

            // 判断是否有 输入输出重定向
            if (sh.m_cmd.left || sh.m_cmd.right)
            {
                int size = sh.m_cmd.cmd.size() - 1;
                char *filename = (char *)sh.m_cmd.cmd.back().c_str();
                char *cmd_name = (char *)sh.m_cmd.cmd.front().c_str();

                char *cmd_arg[size + 1];
                for (size_t i = 0; i < size; i++)
                {
                    cmd_arg[i] = (char *)sh.m_cmd.cmd.front().c_str();
                    sh.m_cmd.cmd.pop_front();
                }
                cmd_arg[size] = nullptr;

                // 包含输入重定向
                if (sh.m_cmd.left)
                {
                    int fd = open(filename, O_RDONLY);
                    if (fd < 0)
                    {
                        printError("open file");
                    }

                    dup2(fd, STDIN_FILENO);
                }
                // 包含输出重定向
                else
                {
                    int fd = open(filename, O_WRONLY | O_CREAT | O_TRUNC, 0664);
                    if (fd < 0)
                    {
                        printError("open file");
                    }

                    dup2(fd, STDOUT_FILENO);
                }

                // 调用exec函数，运行用户输入的命令
                int temp = execvp(cmd_name, cmd_arg);
                if (temp < 0)
                {
                    sh.printError("exec error");
                }
            }
            // 不包含输入输出重定向
            else
            {
                char *cmd_name = (char *)sh.m_cmd.cmd.front().c_str();

                int size = sh.m_cmd.cmd.size();

                char *cmd_arg[size + 1];
                for (size_t i = 0; i < size; i++)
                {
                    cmd_arg[i] = (char *)sh.m_cmd.cmd.front().c_str();
                    sh.m_cmd.cmd.pop_front();
                }
                cmd_arg[size] = nullptr;

                int temp = execvp(cmd_name, cmd_arg);
                if (temp < 0)
                {
                    sh.printError("exec error");
                }
            }
        }
    }

    return 0;
}
