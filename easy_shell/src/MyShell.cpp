#ifndef __FILE_H__
#define __FILE_H__
#include "MyShell.h"
using namespace std;
#endif

MyCommand::MyCommand()
{
}

void MyCommand::clear()
{
    this->cmd.clear();

    this->left = false;
    this->right = false;
}

MyCommand::~MyCommand() {}

MyShell::MyShell()
{
    this->m_temp.clear();
    this->m_cmd_list.clear();
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

void MyShell::pushTemp()
{
    if (this->m_flag == true)
    {
        this->m_cmd.cmd.push_back(this->m_temp);
        this->m_temp.clear();
        this->m_flag = false;
    }
}

void MyShell::split(const char &c)
{
    switch (c)
    {
    case ' ':
        this->pushTemp();
        break;
    case '>':
        this->m_cmd.right = true;
        this->pushTemp();
        break;
    case '<':
        this->m_cmd.left = true;
        this->pushTemp();
        break;
    // case '|':
    //     /* code */
    //     break;
    default:
    {
        this->m_flag = true;
        this->m_temp += c;
    }
    break;
    }
}

void MyShell::recInput()
{
    cout << "Please input the command:" << endl;

    string str;
    getline(cin, str);

    this->m_cmd.clear();
    this->m_temp.clear();
    this->m_flag = false;
    this->m_cmd_list.clear();

    for (string::iterator it = str.begin(); it < str.end(); it++)
    {
        this->split(*it);
    }
    // deal with the end of input
    this->pushTemp();
}

MyShell::~MyShell() {}