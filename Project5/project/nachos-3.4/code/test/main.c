#include "syscall.h"
#include "copyright.h"

#define MAX_LENGTH 32


int main()
{
	int check; // kiem tra thanh cong hay that bai khi mo file
	int n = 0;	// so luong thoi diem xet
	char value;	// gia tri doc tu file
	SpaceId fileInput, fileOutput, fileSV, fileRS;	// id cua cac file input, output, sinhvien, result
	
	// Khoi tao 4 Semaphore
	check = CreateSemaphore("main",0);
	if (check == -1)
	{
		PrintString("tao semaphore that bai\n");
		return 1;
	}

	check = CreateSemaphore("sinhvien", 0);
	if(check == -1)
	{
		PrintString("tao semaphore that bai\n");
		return 1;
	}

	check = CreateSemaphore("voinuoc", 0);
	if(check == -1)
	{
		PrintString("tao semaphore that bai\n");
		return 1;
	}

	check = CreateSemaphore("temp", 0);
	if(check == -1)
	{
		PrintString("tao semaphore that bai\n");
		return 1;
	}
	
	
	// Mo file input.txt de doc type = 1
	fileInput = Open("input.txt", 1);
	if(fileInput == -1){
		PrintString("mo file input that bai\n");
		return 1;
	}

	// Tao file output.txt 
	check = CreateFile("output.txt");
	if(check == -1){
		PrintString("tao file output that bai\n");
		return 1;
	}
	
	// Mo file output.txt de doc va ghi
	fileOutput = Open("output.txt", 0);
	if(fileOutput == -1){
		Close(fileInput);
		return 1;
	}

	// Goi thuc thi tien trinh sinhvien.c
	check = Exec("./test/sinhvien");
	if(check == -1)
	{
		Close(fileInput);
		Close(fileOutput);
		return 1;
	}

	// Goi thuc thi tien trinh voinuoc.c
	check = Exec("./test/voinuoc");
	if(check == -1)
	{
		Close(fileInput);
		Close(fileOutput);
		return 1;
	}

	// Doc so luong thoi diem xet o file input.txt
	while (1) {
		Read(&value, 1, fileInput);
		if (value != '\n') {

			if (value >= '0' && value <= '9')
				n = n * 10 + (value - 48);
		}
		else
			break;
	}
	//lap tung thoi diem
	while(n--)
	{
		// Tao file sinhvien.txt
		check = CreateFile("sinhvien.txt");
		if(check == -1)
		{
			Close(fileInput);
			Close(fileOutput);
			return 1;
		}
		
		// ghi tung dong sinhvien tu file input.txt
		fileSV = Open("sinhvien.txt", 0);
		if(fileSV == -1)
		{
			Close(fileInput);
			Close(fileOutput);
			return 1;
		}
		while(1)
		{
			if(Read(&value, 1, fileInput) < 1)
			{
				// Doc toi cuoi file
				break;
			}
			if(value != '\n')
			{
				Write(&value, 1, fileSV);				
			}
			else
				break;
						
		}
		// Dong file sinhvien.txt lai
		Close(fileSV);
			
		// Goi tien trinh sinhvien hoat dong
		Signal("sinhvien");

		// Tien trinh chinh phai cho 
		Wait("main");	
		
		// Thuc hien doc file tu result va ghi vao ket qua o output.txt
		fileRS = Open("result.txt", 1);
		if(fileRS == -1)
		{
			Close(fileInput);
			Close(fileOutput);
			return 1;
		}

		// ghi ket qua vao output.txt		
		while(1)
		{
			if(Read(&value, 1, fileRS)  < 1)
			{
				Write("\r\n", 2, fileOutput);
				Close(fileRS);
				Signal("temp");
				break;
			}
			Write(&value, 1, fileOutput);
			Write(" ", 1, fileOutput);
		}
	}
	
	Close(fileInput);
	Close(fileOutput);
	return 0;	
	
}
