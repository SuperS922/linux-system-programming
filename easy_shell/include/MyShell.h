#include <iostream>
#include <string>
#include <algorithm>
#include <list>
#include <sys/types.h>
#include <unistd.h>
#include <functional>
#include <map>
#include <sys/wait.h>

using namespace std;

class MyShell
{

public:
    MyShell();
    void myWait(pid_t pid);

    void printError(char *c);

    void recInput();

    ~MyShell();

public:
    pid_t m_pid;

    list<string> m_cmd;

private:
    map<int, list<string>> m_map;

    string m_temp;
    bool m_flag;
};