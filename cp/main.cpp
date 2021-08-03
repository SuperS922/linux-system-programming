#include <iostream>
#include "mult_process_cp.h"

using namespace std;

int main(int argc, char *argv[])
{

    if (argc < 2)
    {
        perror("argument error");
        exit(0);
    }

    clock_t start, end;

    start = clock();
    MyCopy cp(atoi(argv[1]), argv[2], "./copy.txt");

    cp.copy();
    end = clock();

    cout << "run time:" << double(end - start) / CLOCKS_PER_SEC << "s" << endl;

    return 0;
}
