#ifndef __FILE_H__
#define __FILE_H__
#include "MyServer.h"
using namespace std;
#endif

void printError(const char *c)
{
    perror(c);
    exit(0);
}

MyServer::MyServer()
{
}

void MyServer::serverStart()
{
    this->read_fd = open(PUBLIC_FIFO, O_RDONLY);
    if (this->read_fd < 0)
    {
        printError("open public fifo");
    }
}

void MyServer::serverRead()
{
    dup2(this->read_fd, STDIN_FILENO);
    int len = read(STDIN_FILENO, &(this->m_mp), sizeof(this->m_mp));
    if (len < 0)
    {
        printError("read public fifo");
    }
}

bool MyServer::isExist(int id)
{
    if (this->m_member.find(id) != this->m_member.end())
        return true;
    else
        return false;
}

void MyServer::connectAsk()
{
    if (!isExist(this->m_mp.src))
    {
        this->m_member.insert(this->m_mp.src);
    }
    else
    {
        // 连接成功返回 协议号等于2
        this->m_mp.pro_num = 2;
        write((*this->write_fd.find(this->m_mp.src)).second, &(this->m_mp), sizeof(this->m_mp));
        return;
    }

    int temp_fd = open((char *)&(this->m_mp.data), O_WRONLY);
    if (temp_fd < 0)
    {
        printError("open private fifo");
    }

    this->write_fd.insert(make_pair(this->m_mp.src, temp_fd));

    this->m_mp.pro_num = 2;
    write((*this->write_fd.find(this->m_mp.src)).second, &(this->m_mp), sizeof(this->m_mp));
}

void MyServer::sendAsk()
{
    if (!isExist(this->m_mp.src))
    {
        // 尚未建立连接
        return;
    }

    // dst客户不在线
    if (!isExist(this->m_mp.dst))
    {
        this->m_mp.pro_num = 4;
        write((*this->write_fd.find(this->m_mp.src)).second, &(this->m_mp), sizeof(this->m_mp));
    }

    // 输出至 dst 私有fifo
    write((*this->write_fd.find(this->m_mp.dst)).second, &(this->m_mp), sizeof(this->m_mp));
}

void MyServer::quitAsk()
{
    if (isExist(this->m_mp.src))
    {
        // 删除登陆ID
        this->m_member.erase(this->m_member.find(this->m_mp.src));

        // 关闭私有管道
        close((*this->write_fd.find(this->m_mp.src)).second);

        // 删除维护的管道信息
        this->write_fd.erase(this->write_fd.find(this->m_mp.src));
    }
}

void MyServer::serverTrans()
{
    switch (this->m_mp.pro_num)
    {
    case 1:
        connectAsk();
        break;
    case 3:
        sendAsk();
        break;
    case 5:
        quitAsk();
        break;
    default:
        break;
    }
}

void MyServer::run()
{
    this->serverStart();
    while (true)
    {
        this->serverRead();
        this->serverTrans();
    }
}

MyServer::~MyServer()
{
}