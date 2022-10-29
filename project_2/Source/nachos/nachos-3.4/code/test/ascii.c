#include "syscall.h"

int main()
{
    char count;

    printString("--------ASCII TABLE--------");

    for(count = 0; count < 255; count++)
    {
        printInt(count);
        printString(" - ");
        printChar(count);
        printString("\t");
        if(count % 5 == 0)
            printString("\n");
    }

    return 0;
}