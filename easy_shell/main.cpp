#include <iostream>
#include <algorithm>
#include <sys/types.h>
#include <sys/stat.h>
#include <vector>
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

void myPrint(const string str)
{
    cout << str << " ";
}

void printError(const char *c)
{
    perror(c);
    exit(0);
}

void parentExecute(MyShell &sh)
{
    // 父进程负责阻塞式等待子进程的结束

    sh.myWait(sh.m_pid);
}

void childExecute(MyCommand &command)
{
    // 判断是否有 输入输出重定向
    if (command.left || command.right)
    {
        int size = command.cmd.size() - 1;
        char *filename = (char *)command.cmd.back().c_str();
        char *cmd_name = (char *)command.cmd.front().c_str();

        char *cmd_arg[size + 1];
        for (size_t i = 0; i < size; i++)
        {
            cmd_arg[i] = (char *)command.cmd.front().c_str();
            command.cmd.pop_front();
        }
        cmd_arg[size] = nullptr;

        // 包含输入重定向
        if (command.left)
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
            printError("exec error");
        }
    }
    // 不包含输入输出重定向
    else
    {
        char *cmd_name = (char *)command.cmd.front().c_str();

        int size = command.cmd.size();

        char *cmd_arg[size + 1];
        for (size_t i = 0; i < size; i++)
        {
            cmd_arg[i] = (char *)command.cmd.front().c_str();
            command.cmd.pop_front();
        }
        cmd_arg[size] = nullptr;

        int temp = execvp(cmd_name, cmd_arg);
        if (temp < 0)
        {
            printError("exec error");
        }
    }
}

int main()
{
    MyShell sh;
    while (true)
    {
        // 获取用户输入命令并进行拆分，格式为string
        sh.recInput();

        if (sh.m_cmd_list.size() == 0)
        {
            // 无用户输入
            continue;
        }
        else if (sh.m_cmd_list.size() == 1)
        {
            // 无管道符
            MyCommand cmd = sh.m_cmd_list.front();

            // 创建子进程
            sh.m_pid = fork();
            if (sh.m_pid < 0)
            {
                sh.printError("fork error");
            }
            else if (sh.m_pid != 0)
            {
                parentExecute(sh);
            }
            else
            {
                childExecute(cmd);
            }
        }
        else
        {
            // 存在管道符

            int num = sh.m_cmd_list.size() - 1; // 管道符数量
            list<vector<int>> l;

            for (size_t i = 0; i < num; i++)
            {
                int pipefd[2];
                int temp = pipe(pipefd);
                if (temp < 0)
                {
                    printError("pipe error");
                }

                vector<int> v_temp;
                v_temp.push_back(pipefd[0]);
                v_temp.push_back(pipefd[1]);

                l.push_back(v_temp);
            }

            for (size_t j = 0; j < num + 1; j++)
            {
                MyCommand cmd = sh.m_cmd_list.front();

                // 创建子进程
                sh.m_pid = fork();
                if (sh.m_pid < 0)
                {
                    sh.printError("fork error");
                }
                else if (sh.m_pid != 0)
                {
                    if (j > 0)
                    {
                        /* 
                        写端不关闭，但是也不写数据，读端不关闭：
                        此时管道中剩余的数据都被读取之后再次read会被阻塞，
                        直到管道中有数据可读了才重新读取数据并返回；

                        所以必须关闭父进程里面的管道符
                        */
                        close(l.front()[0]);
                        close(l.front()[1]);
                        l.pop_front();
                    }
                    parentExecute(sh);
                }
                else
                {
                    if (j == 0)
                    {
                        vector<int> pipefd = l.front();
                        // 关闭读端
                        close(pipefd[0]);

                        dup2(pipefd[1], STDOUT_FILENO);
                    }
                    else if (j == num)
                    {
                        vector<int> pipefd = l.front();
                        // 关闭写端
                        close(pipefd[1]);

                        dup2(pipefd[0], STDIN_FILENO);
                    }
                    else
                    {
                        vector<int> pipefd_front = l.front();
                        close(pipefd_front[1]); // 关闭前面管道写端
                        dup2(pipefd_front[0], STDIN_FILENO);

                        l.pop_front();
                        vector<int> pipefd_back = l.front();
                        close(pipefd_back[0]); // 关闭后面管道读端
                        dup2(pipefd_back[1], STDOUT_FILENO);
                    }

                    childExecute(cmd);
                }
                sh.m_cmd_list.pop_front();
            }
        }
    }

    return 0;
}
