#include <iostream>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <vector>
#include <cmath>
#include <sys/wait.h>
#include <string.h>
#include <algorithm>

using namespace std;

//1. 指定创建子进程的个数

//2. 打开源文件

//3. 打开目的文件, 不存在则创建

//4. 获取文件大小

//5. 根据文件大小拓展目标文件

//6. 为源文件创建映射

//7. 为目标文件创建映射

//8. 求出每个子进程该拷贝的字节数

//9. 创建N个子进程

//10. 子进程完成分块拷贝(注意最后一个子进程拷贝起始位置)

//11. 释放映射区

class MyCopy
{
public:
    MyCopy(int childNum, char *srcPath, char *dstPath);

    ~MyCopy();

    void copy();

private:
    void openSrcfile();

    void openDstfile();

    void getFileSize();

    void truncDstSize();

    void mmapSrc();

    void mmapDst();

    void mult_cp();

    void release();

private:
    int m_chileNum;
    vector<int> m_pid;

    int m_src_fd;
    char *m_srcPath;
    char *m_srcMmap;

    int m_dst_fd;
    char *m_dstPath;
    char *m_dstMmap;

    int file_size;

    struct stat m_statbuf;
};