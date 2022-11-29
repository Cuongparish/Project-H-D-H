/*main program*/
#include "syscall.h"

int
main()
{
    OpenFileId inp, sv, out, res;
    char c[1];
    int n, ret;
    SpaceId svproc;
    ret = CreateFile("output.txt");
    if (ret == -1) Exit(-1);
    ret = CreateSemaphore("sinhvien", 0);
    if (ret == -1) Exit(-1);
    ret = CreateSemaphore("voinuoc", 0);
    if (ret == -1) Exit(-1);
    inp = Open("input.txt", 1);
    if (inp == -1) Exit(-1);
    out = Open("output.txt", 0);
    if (out == -1) {
        Close(inp);
        Exit(-1);
    }
    n = 0;
    while (1) {
        if (Read(c, 1, inp) == 0) break;
        if (c[0] >= '0' && c[0] <= '9') n = 10 * n + c[0] - '0';
        else if (c[0] == '\n') break;
    }
    while (n--) {
        ret = CreateFile("sinhvien.txt");
        if (ret == -1) {
            Close(inp);
            Exit(-1);
        }
        sv = Open("sinhvien.txt", 0);
        if (sv == -1) {
            Close(inp);
            Exit(-1);
        }
        while (1) {
            if (Read(c, 1, inp) == 0) break;
            Write(c, 1, sv);
            if (c[0] == '\n') break;
        }
        Close(sv);
        svproc = Exec("test/student2");
        if (svproc != -1) ret = Join(svproc);
        if (ret == -1) {
            Close(inp);
            CloseF(out);
            Exit(-1);
        }
        res = OpenF("result.txt", 0);
        if (res == -1) {
            Close(inp);
            Close(out);
            Exit(-1);
        }
        while (Read(c, 1, res) != 0)
            Write(c, 1, out);
        Close(res);
    }
    Exit(0);
}

