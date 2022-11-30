#include "syscall.h"

int main()
{
    char c; // du lieu ky tu doc tu input
    int n; // n: so luong thoi diem
    int check; //bien kiem tra cac thao tac tao, mo file
    SpaceId sv;// nhan gia tri tra ve tu thuc thi tien trinh
    //id cua cac file input, sinhvien, output, result
    OpenFileId fileInput, fileSv, fileOutput, fileRs;

    // tao 2 semaphore de quan ly
    check = CreateSemaphore("sinhvien", 0);
    if (check == -1) {
        PrintString("tao semaphore that bai\n");
        return -1;
    }
    
    check = CreateSemaphore("voinuoc", 0);
    if (check == -1) {
        PrintString("tao semaphore that bai\n");
        return -1;
    }

    // tao file output, doc va mo file input, output
    fileInput = Open("input.txt", 1);
    if (fileInput == -1){
        PrintString("mo file that bai\n");
        return -1;
    }

    check = CreateFile("output.txt");
    if (check == -1)
    {
        PrintString("tao file that bai\n");
        return -1;
    }

    fileOutput = Open("output.txt", 0);
    if (fileOutput == -1) {
        Close(fileInput);
        PrintString("mo file that bai\n");
        return -1;
    }

    // lay so thoi diem tu file input
    n = 0;
    while (1) {
        if (Read(&c, 1, fileInput) == 0) {
            break;
        }// doc tung ky tu cua so
        if (c >= '0' && c <= '9') {
            n = 10 * n + c - '0';
        }
        else if (c == '\n') {
            break;
        }
    }

    //lan luot duyet qua tung thoi diem
    while (n--) {

        //tao va mo file sinhvien.txt
        check = CreateFile("sinhvien.txt");
        if (check == -1) {
            Close(fileInput);
            return -1;
        }

        fileSv = Open("sinhvien.txt", 0);
        if (fileSv == -1) {
            Close(fileInput);
            return -1;
        }

        //lay so the tich cua tung sinh vien tai thoi diem dang xet
        while (1) {
            if (Read(&c, 1, fileInput) == 0) {
                break;
            }
            Write(&c, 1, fileSv);
            if (c == '\n') {
                break;
            }
        }
        Close(fileSv);

        //goi thuc thi tien trinh sinhvien
        sv = Exec("./test/sinhvien");
        if (sv != -1) check = Join(sv);
        if (check == -1) {
            Close(fileInput);
            Close(fileOutput);
            return -1;
        }

        // mo file result de doc ket qua va ghi vao file output
        fileRs = Open("result.txt", 0);
        if (fileRs == -1) {
            Close(fileInput);
            Close(fileOutput);
            return -1;
        }
        while (Read(&c, 1, fileRs) != 0)
        {
            Write(&c, 1, fileOutput);
        }
        Close(fileRs);
    }
    Exit(0);
}

