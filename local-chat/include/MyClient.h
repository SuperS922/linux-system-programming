#include <iostream>
#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <string>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/stat.h>
#include "MyProtocol.h"
using namespace std;

extern char **environ;

class MyClient
{
public:
    MyClient();

    void run();

    ~MyClient();

private:
    void showMenu();

    void setSrcNum();

    void setDstNum();

    void connect();

    void connectToServer();

    void send(const char *msg);

    void chat();

    void exit();

    void exitConnect();

private:
    int write_fd;
    int read_fd;

    bool is_connect;

    MyProtocol m_readmp;
    MyProtocol m_writemp;
};
