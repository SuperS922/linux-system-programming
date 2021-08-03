#ifndef __FILE_H__
#define __FILE_H__
#include <mult_process_cp.h>
using namespace std;
#endif

void printError(const char *msg)
{
    perror(msg);
    exit(0);
}

void myWait(int i)
{
    int status;
    waitpid(i, &status, 0); // 主要掌握这里：waitpid 的使用方法

    if (WIFEXITED(status))
    {
        cout << "child " << i << " exit with " << WEXITSTATUS(status) << endl;
    }
    else if (WIFSIGNALED(status))
    {
        cout << "child " << i << " killed by " << WTERMSIG(status) << endl;
    }
}

MyCopy::MyCopy(int childNum, char *srcPath, char *dstPath)
{
    if (childNum < 0 || childNum > 10)
    {
        cout << "num of child process is not between 0 and 10." << endl;
        exit(0);
    }

    this->m_chileNum = childNum;
    this->m_srcPath = srcPath;
    this->m_dstPath = dstPath;
}

void MyCopy::openSrcfile()
{
    this->m_src_fd = open(m_srcPath, O_RDONLY);

    if (m_src_fd < 0)
    {
        printError("open src file");
    }

    cout << "file open success" << endl;
}

void MyCopy::openDstfile()
{
    this->m_dst_fd = open(m_dstPath, O_RDWR | O_CREAT | O_TRUNC, 0664);

    if (m_dst_fd < 0)
    {
        printError("open dst file");
    }
}

void MyCopy::getFileSize()
{
    int temp = fstat(this->m_src_fd, &m_statbuf);
    if (temp < 0)
    {
        printError("get file size");
    }

    this->file_size = m_statbuf.st_size;
}

void MyCopy::truncDstSize()
{
    int temp = ftruncate(m_dst_fd, file_size);

    if (temp < 0)
    {
        printError("truncate file error");
    }
}

void MyCopy::mmapSrc()
{
    m_srcMmap = (char *)mmap(nullptr, file_size, PROT_READ, MAP_SHARED, m_src_fd, 0);
    if (m_srcMmap == MAP_FAILED)
    {
        printError(" src file mmap");
    }

    close(m_src_fd);
}

void MyCopy::mmapDst()
{
    m_dstMmap = (char *)mmap(nullptr, file_size, PROT_READ | PROT_WRITE, MAP_SHARED, m_dst_fd, 0);
    if (m_dstMmap == MAP_FAILED)
    {
        printError(" dst file mmap");
    }

    close(m_dst_fd);
}

void MyCopy::mult_cp()
{
    int index = ceil((float)file_size / (float)4096 / m_chileNum);
    int length = index * 4096;
    for (size_t i = 0; i < m_chileNum; i++)
    {
        pid_t pid;

        pid = fork();

        if (pid < 0)
        {
            printError("fork error");
        }
        else if (pid != 0)
        {
            m_pid.push_back(pid);
        }
        else
        {
            // child process
            cout << "child process " << i + 1 << "start copy..." << endl;
            if (i == m_chileNum - 1)
            {
                strncpy(&m_dstMmap[length * i], &m_srcMmap[length * i], file_size - length * i);
            }
            else
            {
                strncpy(&m_dstMmap[length * i], &m_srcMmap[length * i], length);
            }
            exit(0);
        }
    }
    for_each(m_pid.begin(), m_pid.end(), myWait);
}

void MyCopy::release()
{
    munmap(m_srcMmap, file_size);
    munmap(m_dstMmap, file_size);
}

void MyCopy::copy()
{
    openSrcfile();

    openDstfile();

    getFileSize();

    truncDstSize();

    mmapSrc();

    mmapDst();

    mult_cp();

    release();
}

MyCopy::~MyCopy()
{
}