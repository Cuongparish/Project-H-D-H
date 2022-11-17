#include "syscall.h"

void swap(int* xp, int* yp)
{
    int temp = *xp;
    *xp = *yp;
    *yp = temp;
}

void bubbleSort(int arr[], int n)
{
    int i, j;
    for (i = 0; i < n - 1; i++)
 
        // Last i elements are already in place
        for (j = 0; j < n - i - 1; j++)
            if (arr[j] > arr[j + 1])
                swap(&arr[j], &arr[j + 1]);
}

void printArray(int arr[], int size)
{
    int i;
    for (i = 0; i < size; i++)
    {
        printInt(arr[i]);
	printString("  ");
    }
    printString("\n");
}

void readArray(int arr[], int* size)
{
    int i;
    do
    {
        printString("Nhập số lượng phần tử của mảng: ");
        *size = readInt();
    } while (*size < 0 || *size > 100);
    
    for(i = 0; i < *size; i++)
    {
        printString("Nhập phần tử thứ ");
        printInt(i + 1);
        printString(": ");
        arr[i] = readInt();
    }
}

int main()
{
    int arr[100];
    int size;

    readArray(arr, &size);
    printString("Mảng của bạn: ");
    printArray(arr, size);
    
    bubbleSort(arr, size);
    printString("Mảng sau khi sắp xếp: ");
    printArray(arr, size);
    Halt();
    return 0;
}
