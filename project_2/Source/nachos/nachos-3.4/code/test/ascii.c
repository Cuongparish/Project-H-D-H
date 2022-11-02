#include "syscall.h"

int main()
{
    char item = 0;
    int i;

    printString("--------ASCII TABLE--------\n");

    for(i = 0; i < 255; i++)
    {
        printInt(i);
        printString(" - ");
        printChar(item);
        printString("\t");
	if(i % 6 == 0)
            printString("\n");
	item++;
    }

    return 0;
}