#include <iostream>
#include <string>
#include <algorithm>
#include <list>
#include <sys/types.h>
#include <unistd.h>
#include <sys/wait.h>

using namespace std;

/*
实现步骤： 
1. 接收用户输入命令字符串，拆分命令及参数存储。（自行设计数据存储结构）
2. 实现普通命令加载功能 
3. 实现输入、输出重定向的功能 
4. 实现管道 
5. 支持多重管道
*/

void myWait(pid_t pid)
{
    int status;
    waitpid(pid, &status, 0); // 主要掌握这里：waitpid 的使用方法

    if (WIFEXITED(status))
    {
        cout << "child " << pid << " exit with " << WEXITSTATUS(status) << endl;
    }
    else if (WIFSIGNALED(status))
    {
        cout << "child " << pid << " killed by " << WTERMSIG(status) << endl;
    }
}

void printError(char *c)
{
    perror(c);
    exit(0);
}

void myPrint(string str)
{
    cout << str << " ";
}

void recInput(list<string> &command)
{
    cout << "Please input the command:" << endl;

    char c_temp;
    string s_temp;
    bool flag = false;

    while (true)
    {
        c_temp = cin.get();
        if (c_temp == ' ' || c_temp == '\n')
        {
            if (flag == false)
            {
                if (c_temp == '\n')
                {
                    // end of input
                    break;
                }
                continue;
            }
            else
            {
                flag = false;
                command.push_back(s_temp);
                s_temp.clear();
                if (c_temp == '\n')
                {
                    // end of input
                    break;
                }
            }
        }
        else
        {
            flag = true;
            s_temp += c_temp;
        }
    }
}

int main()
{
    pid_t pid;
    list<string> cmd;
    while (true)
    {
        cmd.clear();
        recInput(cmd);

        pid = fork();
        if (pid < 0)
        {
            printError("fork error");
        }
        else if (pid != 0)
        {
            // parent process

            cout << "output -------------------" << endl;
            for_each(cmd.begin(), cmd.end(), myPrint);
            cout << endl;

            myWait(pid);
        }
        else
        {
            // child process
            char *cmd_name = (char *)cmd.front().c_str();

            int size = cmd.size();

            char *cmd_arg[size + 1];
            for (size_t i = 0; i < size; i++)
            {
                cmd_arg[i] = (char *)cmd.front().c_str();
                cmd.pop_front();
            }
            cmd_arg[size] = nullptr;

            int temp = execvp(cmd_name, cmd_arg);
            if (temp < 0)
            {
                printError("exec error");
            }
        }
    }

    return 0;
}
