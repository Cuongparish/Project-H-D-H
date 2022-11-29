/*voinuoc program*/
#include "syscall.h"

int
main()
{
    int n, v1, v2;
    OpenFileId vn, res;
    char c[1];
    CreateFile("result.txt");
    res = Open("result.txt", 0);
    n = -1;
    v1 = v2 = 0;
    while (1) {
        if (n == 0) break;
        Wait("voinuoc");
        vn = Open("voinuoc.txt", 1);
        n = 0;
        while (1) {
            if (Read(c, 1, vn) == 0) break;
            if (c[0] >= '0' && c[0] <= '9') n = 10 * n + c[0] - '0';
            else break;
        }
        Close(vn);
        if (n != 0) {
            if (v1 <= v2) {
                v1 += n;
                if (res != -1)
                    Write("1 ", 2, res);
            }
            else {
                v2 += n;
                if (res != -1)
                    Write("2 ", 2, res);
            }
        }
        Signal("sinhvien");
    }
    if (res != -1) {
        Write("\n", 1, res);
        Close(res);
    }
    Exit(0);
}
