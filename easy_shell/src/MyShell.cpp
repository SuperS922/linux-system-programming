#ifndef __FILE_H__
#define __FILE_H__
#include "MyShell.h"
using namespace std;
#endif

MyShell::MyShell()
{
    this->m_temp.clear();
    this->m_map.clear();
    this->m_flag = false;
    this->m_cmd.clear();
    this->m_pid = 0;
}

void MyShell::myWait(pid_t m_pid)
{
    int status;
    waitpid(m_pid, &status, 0); // 主要掌握这里：waitpid 的使用方法

    if (WIFEXITED(status))
    {
        cout << "child " << m_pid << " exit with " << WEXITSTATUS(status) << endl;
    }
    else if (WIFSIGNALED(status))
    {
        cout << "child " << m_pid << " killed by " << WTERMSIG(status) << endl;
    }
}

void MyShell::printError(char *c)
{
    perror(c);
    exit(0);
}

void MyShell::recInput()
{
    cout << "Please input the command:" << endl;

    string str;
    getline(cin, str);

    this->m_cmd.clear();
    this->m_temp.clear();
    this->m_flag = false;

    for (string::iterator it = str.begin(); it < str.end(); it++)
    {
        switch (*it)
        {
        case ' ':
        {
            if (this->m_flag == true)
            {
                this->m_cmd.push_back(this->m_temp);
                this->m_temp.clear();
                this->m_flag = false;
            }
        }
        break;
        // case '>':
        //     /* code */
        //     break;
        // case '<':
        //     /* code */
        //     break;
        // case '|':
        //     /* code */
        //     break;
        default:
        {
            this->m_flag = true;
            this->m_temp += *it;
        }
        break;
        }
    }
    // deal with the end of input
    if (this->m_flag == true)
    {
        this->m_cmd.push_back(this->m_temp);
    }
}

MyShell::~MyShell() {}