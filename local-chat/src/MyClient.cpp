#ifndef __FILE_H__
#define __FILE_H__
#include "MyClient.h"
using namespace std;
#endif

void printErr(const char *c)
{
    perror(c);
    exit(0);
}

MyClient::MyClient()
{
    this->is_connect = false;
    this->m_writemp.src = -1;
    this->m_writemp.dst = -1;

    write_fd = open(PUBLIC_FIFO, O_WRONLY);
    if (write_fd < 0)
    {
        printErr("open public fifo");
    }
}

void MyClient::setSrcNum()
{
    system("reset");
    int temp;
    cout << "please input your room ID:" << endl;
    cin >> temp;

    if (temp <= 0)
    {
        cout << "room ID must bigger than 0, please try again" << endl;
    }
    else
    {
        this->m_writemp.src = temp;
        cout << "room ID set success:" << this->m_writemp.src << endl;
    }
    cin.get();
}

void MyClient::setDstNum()
{
    system("reset");
    int temp;
    cout << "please input friend's room ID:" << endl;
    cin >> temp;

    if (temp <= 0)
    {
        cout << "friend's room ID must bigger than 0, please try again" << endl;
    }
    else
    {
        this->m_writemp.dst = temp;
        cout << "friend's room ID set success:" << this->m_writemp.dst << endl;
    }
    cin.get();
}

void MyClient::connect()
{
    this->m_writemp.pro_num = 1;

    char *dirname = getenv("PWD");

    char *pathname = strcat(strcat(dirname, (char *)"./fifo"), to_string(this->m_writemp.src).c_str());

    // 新建一个fifo
    int temp = mkfifo(pathname, 0664);
    if (temp < 0)
    {
        printErr("create private pipe");
    }

    // 绑定私有fifo的读端
    read_fd = open(pathname, O_RDONLY | O_NONBLOCK);
    unlink(pathname);

    // 把文件名传给 server
    strcpy(this->m_writemp.data, pathname);
    int len = write(write_fd, &this->m_writemp, sizeof(this->m_writemp));
    if (len < 0)
    {
        perror("client connect write error");
    }

    sleep(1);

    len = read(read_fd, &this->m_readmp, sizeof(this->m_readmp));
    if (len < 0)
    {
        perror("client connect read error");
    }

    if (this->m_readmp.pro_num == 2)
    {
        this->is_connect = true;
    }
}

void MyClient::connectToServer()
{
    if (this->is_connect == true)
    {
        cout << "It has connected." << endl;
        cin.get();
        return;
    }
    if (this->m_writemp.src < 0)
    {
        cout << "You had not set your room ID" << endl;
        cin.get();
        return;
    }
    if (this->m_writemp.dst < 0)
    {
        cout << "You had not set your friend's room ID" << endl;
        cin.get();
        return;
    }

    this->connect();
    cout << "Connect success." << endl;
    cin.get();
    return;
}

void MyClient::chat()
{
    system("reset");
    if (this->is_connect == false)
    {
        cout << "It has not connected." << endl;
        cin.get();
        return;
    }

    fcntl(STDIN_FILENO, O_NONBLOCK);
    char *msg;
    cout << "Send to " << this->m_writemp.dst << ":\t";
    while (true)
    {
        int len = read(read_fd, &this->m_readmp, sizeof(this->m_readmp));
        if (len > 0)
        {
            cout << "From " << this->m_readmp.src << ":\t" << this->m_readmp.data << endl;
        }

        len = read(STDIN_FILENO, msg, sizeof(this->m_writemp));
        if (len <= 0)
        {
            break;
        }

        this->send(msg);
    }

    cout << "chat finished." << endl;
}

void MyClient::send(const char *msg)
{
    // 3 代表数据发送包
    this->m_writemp.pro_num = 3;
    strcpy(this->m_writemp.data, msg);

    int len = write(write_fd, &this->m_writemp, sizeof(this->m_writemp));
    if (len < 0)
    {
        perror("client connect write error");
    }
}

void MyClient::exit()
{
    this->m_writemp.pro_num = 5;
    int len = write(write_fd, &this->m_writemp, sizeof(this->m_writemp));
    if (len < 0)
    {
        perror("client connect write error");
    }

    this->is_connect = false;
    close(read_fd);
}

void MyClient::exitConnect()
{
    system("reset");
    if (this->is_connect == false)
    {
        cout << "It has not connected." << endl;
        cin.get();
        return;
    }
    this->exit();
}

void MyClient::showMenu()
{
    cout << "---------------------------------------------" << endl;
    cout << "-----Welcome to the local chat room----------" << endl;
    cout << "---------------------------------------------" << endl;
    cout << "--------1. connect to server    -------------" << endl;
    cout << "--------2. set my room ID       -------------" << endl;
    cout << "--------3. set friend's ID      -------------" << endl;
    cout << "--------4. start chat           -------------" << endl;
    cout << "--------5. close connect        -------------" << endl;
    cout << "--------0. qiut                 -------------" << endl;
    cout << "---------------------------------------------" << endl;
}

void MyClient::run()
{
    int user_input;
    while (true)
    {
        this->showMenu();

        cin >> user_input;

        switch (user_input)
        {
        case 1:
            this->connectToServer();
            break;
        case 2:
            this->setSrcNum();
            break;
        case 3:
            this->setDstNum();
            break;
        case 4:
            this->chat();
            break;
        case 5:
            this->exitConnect();
            break;
        case 0:
            exit();
            break;
        default:
            break;
        }
    }
}

MyClient::~MyClient()
{
}