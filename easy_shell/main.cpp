#include <iostream>
#include <algorithm>
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

int main()
{
    MyShell sh;
    while (true)
    {
        sh.recInput();

        sh.m_pid = fork();
        if (sh.m_pid < 0)
        {
            sh.printError("fork error");
        }
        else if (sh.m_pid != 0)
        {
            // parent process

            cout << "output -------------------" << endl;
            for_each(sh.m_cmd.begin(), sh.m_cmd.end(), myPrint);
            cout << endl;

            sh.myWait(sh.m_pid);
        }
        else
        {
            // child process
            char *cmd_name = (char *)sh.m_cmd.front().c_str();

            int size = sh.m_cmd.size();

            char *cmd_arg[size + 1];
            for (size_t i = 0; i < size; i++)
            {
                cmd_arg[i] = (char *)sh.m_cmd.front().c_str();
                sh.m_cmd.pop_front();
            }
            cmd_arg[size] = nullptr;

            int temp = execvp(cmd_name, cmd_arg);
            if (temp < 0)
            {
                sh.printError("exec error");
            }
        }
    }

    return 0;
}
