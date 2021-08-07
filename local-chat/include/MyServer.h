#include <iostream>
#include <set>
#include <map>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include "MyProtocol.h"

using namespace std;

class MyServer
{
public:
    MyServer();

    void run();

    ~MyServer();

private:
    void serverStart();

    void serverRead();

    void serverTrans();

    bool isExist(int id);

    void connectAsk();

    void sendAsk();

    void quitAsk();

private:
    // 公共fifo的读端
    int read_fd;
    // 私有fifo的写端  <用户ID，文件描述符FD>
    map<int, int> write_fd;

    // 用于接收用户的输入，并输出
    MyProtocol m_mp;
    // 储存已登录的用户ID
    set<int> m_member;
};
