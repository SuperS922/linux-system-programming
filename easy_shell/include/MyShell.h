#include <iostream>
#include <string>
#include <algorithm>
#include <list>
#include <sys/types.h>
#include <unistd.h>
#include <sys/wait.h>

using namespace std;

struct MyCommand
{
    list<string> cmd;

    bool left;
    bool right;

    MyCommand();

    void clear();

    ~MyCommand();
};

class MyShell
{

public:
    MyShell();
    void myWait(pid_t pid);

    void printError(char *c);

    void split(const char &c);

    void pushTemp();

    void recInput();

    ~MyShell();

public:
    pid_t m_pid;

    MyCommand m_cmd;

private:
    list<MyCommand> m_cmd_list;

    string m_temp;
    bool m_flag;
};