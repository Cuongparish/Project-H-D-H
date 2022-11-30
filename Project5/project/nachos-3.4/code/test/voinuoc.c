/*voinuoc program*/
#include "syscall.h"

int main()
{
    char c;// bien luu gia tri ky tu doc tu input
    int thetich, voi1, voi2;// thetich: the tich binh nuoc cua sinh vien
                            //voi1, voi2: 2 voi nuoc
    OpenFileId fileVn, fileRs;// id cua file voinuoc.txt, result.txt
    int check;// bien kiem tra tao, mo file
    int count; // bien dem so ky tu cua the tich binh nuoc

    // tao va mo file result
    check = CreateFile("result.txt");
    if (check == -1) {
        PrintString("tao file that bai\n");
        return -1;
    }

    fileRs = Open("result.txt", 0);
    if (fileRs == -1) {
        PrintString("mo file that bai\n");
        return -1;
    }

    thetich = -1;
    voi1 = voi2 = 0;
    // lan luot duyet qua tung the tich tai thoi diem dang xet
    while (1) {
        if (thetich == 0)
        {
            break;
        }
        
        //dung thuc thi voinuoc quay lai thucthi tiep sinhvien
        Wait("voinuoc");
        fileVn = Open("voinuoc.txt", 1);
        if (fileVn == -1) {
            PrintString("mo file that bai\n");
            return -1;
        }

        thetich = 0;
        count = 0;
        // lay the tich binh nuoc tu file voinuoc.txt
        while (1) {
            if (Read(&c, 1, fileVn) == 0) { 
                break; }
            if (c >= '0' && c <= '9')
            {
                thetich = 10 * thetich + c - '0';
                count = count + 1;
            }
            else break;
        }
        Close(fileVn);

        //kiem tra voi nuoc rot xong truoc thi cho voi nuoc do rot vao binh moi
        if (thetich != 0) {
            if (voi1 <= voi2) {
                voi1 += thetich;
                if (fileRs != -1)
                {
                    Write(&thetich, count, fileRs);
                    Write("1 ", 2, fileRs);
                }
            }
            else {
                voi2 += thetich;
                if (fileRs != -1)
                {
                    Write(&thetich, count, fileRs);
                    Write("2 ", 2, fileRs);
                }
            }
            Write("    ", 4, fileRs);
        }
        //goi lai chuong trinh sinhvien de tiep tuc
        Signal("sinhvien");
    }
    // kiem tra xem da xu ly xong 1 thoi diem chua
    if (fileRs != -1) {
        Write("\n", 1, fileRs);
        Close(fileRs);
    }
    Exit(0);
}

