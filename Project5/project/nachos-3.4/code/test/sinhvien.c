#include "syscall.h"

intmain()
{
    char c;// luu gia tri ky tu doc tu input
    int check;//kiem tra tao, mo file thanh cong hay khong
    SpaceId vn;// bien nhan gia tri tra ve tu cac tien trinh
    OpenFileId fileSv, fileVn;// id cua file sinhvien va voinuoc

    //
    fileSv = Open("sinhvien.txt", 1);
    if (fileSv == -1) {
        PrintString("mo file that bai\n");
        return -1;
    }

    //thuc thi tien trinh voinuoc
    vn = Exec("./test/voinuoc");
    if (vn == -1) {
        return -1;
    }

    check = -1;
    // lan luot doc tung the tich vao file voinuoc.txt va goi thuc thi chuong trinh voinuoc
    while (check != 0) {

        check = CreateFile("voinuoc.txt");
        if (check == -1) {
            Close(fileSv);
            PrintString("tao file that bai\n");
            return -1;
        }
        fileVn = Open("voinuoc.txt", 0);
        if (fileVn == -1) {
            Close(fileSv);
            PrintString("mo file that bai\n");
            return -1;
        }

        // doc the tich cua 1 bình
        while (1) {
            if (Read(&c, 1, fileSv) == 0) {
                Write("0", 1, fileVn);
                check = 0;
                break;
            }
            else check = -1;
            if (c == ' ' || c == '\n')
            {
                break;
            }
            Write(&c, 1, fileVn);
        }
        Close(fileVn);
        // sau khi doc xong the tich 1 binh
        // goi thuc thi chuong trinh voinuoc va cho sinhvien doi
        Signal("voinuoc");
        Wait("sinhvien");
    }
    Exit(0);
}

