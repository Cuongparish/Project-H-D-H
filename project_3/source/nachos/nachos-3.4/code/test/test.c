#include "syscall.h"
#define maxlen 255

int main()
{
    int i;
    printString("Nhập số nguyên: ");
    i = readInt();
    printString("Số nguyên bạn đã nhập: ");
    printInt(i);

    //char c;
    //printString("Nhập một ký tự: ");
    //c = readChar();
    //printString("Ký tự bạn đã nhập: ");
    //printChar(c);
    
    // char* str;
    // printString("Nhập chuỗi: ");
    // readString(str, 5);
    // printString("5 ký tự đầu của chuỗi: ");
    // printString(str);

    Halt();
    return 0;
}