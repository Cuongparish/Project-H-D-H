#include "syscall.h"
#include "copyright.h"

void main()
{
	int checkFile; // kiem tra mo file thanh cong hay that bai
	char value; // doc gia tri tu file
	int checkVoinuoc; // nhay den voi nuoc
	int lengthFile; // doc dai cua file
	int cursorFile; // con tro chay trong file
	SpaceId fileSV, fileVN; // id cua file sinhvien va voinuoc

	Signal("temp");	

	while(1)
	{
		lengthFile = 0;
		Wait("sinhvien");
		
		// Tao file result.txt de ghi voi nao su dung
		checkFile = CreateFile("result.txt");
		if(checkFile == -1)
		{
			Signal("main"); // tro ve tien trinh chinh
			return;
		}

		fileSV = Open("sinhvien.txt", 1);
		if(fileSV == -1)
		{
			Signal("main"); // tro ve tien trinh chinh
			return;
		}
		
		lengthFile = Seek(-1, fileSV);
		Seek(0, fileSV);
		cursorFile = 0;
	
		checkFile = CreateFile("voinuoc.txt");
		if(checkFile == -1)
		{
			Close(fileSV);
			Signal("main"); // tro ve tien trinh chinh
			return;
		}
		

		// ghi tung dung tich nuoc cua sinhvien
		fileVN = Open("voinuoc.txt", 0);
		if(fileVN == -1)
		{
			Close(fileSV);
			Signal("main"); // tro ve tien trinh chinh
			return;
		}
		
		// ghi dung tich vao file voinuoc.txt tu file sinhvien.txt
		while(cursorFile < lengthFile)
		{
			checkVoinuoc = 0;
			Read(&value, 1, fileSV);
			if(value != ' ')
			{
				Write(&value, 1, fileVN);
			}
			else
			{
				checkVoinuoc = 1;
			}
			if(cursorFile == lengthFile - 1)
			{
				Write("-", 1, fileVN);
				checkVoinuoc = 1;
			}
			
				
			if(checkVoinuoc == 1)
			{
				Close(fileVN);
				Signal("voinuoc");
				// dua chuong trinh sinh vien de voinuoc thuc thi
				Wait("sinhvien");

				checkFile = CreateFile("voinuoc.txt");
				if(checkFile == -1)
				{
					Close(fileSV);
					Signal("main"); // tro ve tien trinh chinh
					return;
				}
		

				// ghi tung dung tich nuoc cua sinhvien
				fileVN = Open("voinuoc.txt", 0);
				if(fileVN == -1)
				{
					Close(fileSV);
					Signal("main"); // tro ve tien trinh chinh
					return;
				}
			}
			cursorFile++;				
		}				
		// Ket thuc tien trinh sinhvien va voinuoc quay lai tien trinh main
		Signal("main");			
	}
}
