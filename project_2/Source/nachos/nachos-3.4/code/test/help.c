#include "syscall.h"

int main()
{
    //Mô tả về nhóm
    printString("--------Thông tin nhóm--------");
    printString("\t20120383-Nguyễn Đức Tiến\n");
    printString("\t20120443-Nguyễn Chí Công\n");
    printString("\t20120444-Nguyễn Tấn Chữ\n");
    printString("\t20120446-Nguyễn Đình Cường\n");
    printString("\t20120447-Trịnh Quốc Cường\n");

    //Mô tả về chương trình help
    printString("--------HELP--------");
    printString("1. Chương trình ascii: \n");
    printString("Chương trình này sẽ xuất ra bảng ascii \n");
    printString("Để chạy chương trình bạn cần nhập lệnh: \n");
    printString("'./userprog/nachos -rs 1023 -x ./test/ascii'");
    printString("2. Chương trình sort: \n");
    printString("Chương trình sort sẽ dùng thuật toán BubbleSort \n");
    printString("Để sử dụng chương trình bạn cần nhập vào 1 mảng để\n");
    printString("Chương trình sẽ lấy thông tin từ mảng đó và sắp xếp sau đó xuất kết quả\n");
    printString("Để chạy chương trình sort bạn cần nhập lệnh: \n");
    printString("'./userprog/nachos -rs 1023 -x ./test/bubblesort'");

    return 0;
}

