// exception.cc
//	Entry point into the Nachos kernel from user programs.
//	There are two kinds of things that can cause control to
//	transfer back to here from user code:
//
//	syscall -- The user code explicitly requests to call a procedure
//	in the Nachos kernel.  Right now, the only function we support is
//	"Halt".
//
//	exceptions -- The user code does something that the CPU can't handle.
//	For instance, accessing memory that doesn't exist, arithmetic errors,
//	etc.
//
//	Interrupts (which can also cause control to transfer from user
//	code into the Nachos kernel) are handled elsewhere.
//
// For now, this only handles the Halt() system call.
// Everything else core dumps.
//
// Copyright (c) 1992-1993 The Regents of the University of California.
// All rights reserved.  See copyright.h for copyright notice and limitation
// of liability and disclaimer of warranty provisions.

#include "copyright.h"
#include "system.h"
#include "syscall.h"
//-----------------
#include <cmath>
#include <iostream>
//-----------------
#define MaxFileLength 32

//----------------------------------------------------------------------
// ExceptionHandler
// 	Entry point into the Nachos kernel.  Called when a user program
//	is executing, and either does a syscall, or generates an addressing
//	or arithmetic exception.
//
// 	For system calls, the following is the calling convention:
//
// 	system call code -- r2
//		arg1 -- r4
//		arg2 -- r5
//		arg3 -- r6
//		arg4 -- r7
//
//	The result of the system call, if any, must be put back into r2.
//
// And don't forget to increment the pc before returning. (Or else you'll
// loop making the same system call forever!
//
//	"which" is the kind of exception.  The list of possible exceptions
//	are in machine.h.
//----------------------------------------------------------------------

// ham tang program counter truoc khi system call tra ket qua
void IncreasePC()
{
	int counter = machine->ReadRegister(PCReg);
	machine->WriteRegister(PrevPCReg, counter);

	counter = machine->ReadRegister(NextPCReg);
	machine->WriteRegister(PCReg, counter);

	machine->WriteRegister(NextPCReg, counter + 4);
}

// Input: Khong gian dia chi User(int) - gioi han cua buffer(int)
// Output: Bo nho dem Buffer(char*)
// Chuc nang: Sao chep vung nho User sang vung nho System
char* User2System(int virtAd, int limit)
{
	int n; // chi so index
	int oneChar;
	char* kernelBuf = NULL;
	kernelBuf = new char[limit + 1]; // can cho chuoi terminal
	if (kernelBuf == NULL)
		return kernelBuf;

	memset(kernelBuf, 0, limit + 1);

	for (n = 0; n < limit; n++)
	{
		machine->ReadMem(virtAd + n, 1, &oneChar);
		kernelBuf[n] = (char)oneChar;
		if (oneChar == 0)
			break;
	}
	return kernelBuf;
}

// Input: Khong gian vung nho User(int) - gioi han cua buffer(int) - bo nho dem buffer(char*)
// Output: So byte da sao chep(int)
// Chuc nang: Sao chep vung nho System sang vung nho User
int System2User(int virtAd, int len, char* buffer)
{
	if (len < 0)
		return -1;
	if (len == 0)
		return len;
	int i = 0;
	int oneChar = 0;
	do
	{
		oneChar = (int)buffer[i];
		machine->WriteMem(virtAd + i, 1, oneChar);
		i++;
	} while (i < len && oneChar != 0);
	return i;
}

// Function to handle run-time errors
void ExceptionHandler(ExceptionType which)
{
	int type = machine->ReadRegister(2);

	switch (which)
	{
	case NoException: //----------------------
		return;

	case PageFaultException: //---------------------
		printf("No valid translation found \n\n");
		ASSERT(FALSE);
		interrupt->Halt();

		break;

	case ReadOnlyException:
		printf("Write attempted to page marked \"read - only\" \n\n");
		ASSERT(FALSE);
		interrupt->Halt();

		break;

	case BusErrorException: //---------------
		printf("Translation resulted in an invalid physical address \n\n");
		ASSERT(FALSE);
		interrupt->Halt();
		break;

	case AddressErrorException: //-------------------
		printf("Unaligned reference or one that was beyond the end of the address space \n \n");
		ASSERT(FALSE);
		interrupt->Halt();
		break;

	case OverflowException: //----------------------
		printf("Integer overflow in add or sub. \n \n");
		ASSERT(FALSE);
		interrupt->Halt();
		break;

	case IllegalInstrException: //-----------------------
		printf("Unimplemented or reserved instr. \n\n");
		ASSERT(FALSE);
		interrupt->Halt();
		break;

	case NumExceptionTypes: //-----------------------
		printf("Number exception types \n\n");
		ASSERT(FALSE);
		interrupt->Halt();
		break;

		// khi cai cac ham systemcall phai xu li loi va in ra
	case SyscallException:
	{
		switch (type)
		{
		case SC_Halt:
			printf("\nShutdown, initiated by user program. ");
			interrupt->Halt();
			break;

		case SC_readChar:
		{
			// goi ham readChar

			char* buffer = new char[255];
			int len = gSynchConsole->Read(buffer, 1);

			if (len == -1)
			{
				printf("\n Error reading character\n");
				break;
			}

			machine->WriteRegister(2, buffer[0]);

			delete[] buffer;

			break;
		}

		case SC_printChar:
		{
			// goi ham printChar
			char c = (char)machine->ReadRegister(4);
			gSynchConsole->Write(&c, 1);
			break;
		}

		case SC_readInt:
		{
			// goi ham readInt
			// doc so nguyen tu man hinh console
			char* buffer = new char[255];
			int len = gSynchConsole->Read(buffer, 256);

			// Xu ly chuoi
			bool check = TRUE;

			int i = 0;
			if (buffer[0] == '-')
			{
				i++;
			}

			for (; i < len; i++)
			{
				// neu khong phai la so thi ghi vao so 0
				if (buffer[i] < '0' || buffer[i] > '9')
				{
					machine->WriteRegister(2, 0);
					printf("\n Not a integer \n");
					check = FALSE;
					break;
				}
			}

			if (check == FALSE)
			{
				break;
			}

			// chuyen lai ve kieu int
			int res = atoi(buffer);

			// doc vao thanh ghi so 2
			machine->WriteRegister(2, res);

			delete[] buffer;
			break;
		}

		case SC_printInt:
		{
			// goi ham printInt
			//  doc tu thanh ghi so 4
			int number = machine->ReadRegister(4);

			if (number == 0) // nguoi dung nhap vao khong phai so thi in ra so 0
			{
				gSynchConsole->Write("0", 1);
				break;
			}

			// kiem tra xem so am hay duong
			int check = TRUE;
			if (number < 0)
			{
				check = TRUE;
			}
			else
			{
				check = FALSE;
			}
			// neu la so am chuyen lai thanh so duong(*-1), neu la so duong (*1)
			number *= 1 - check * 2;

			// chieu dai number khi number o dang so duong
			int numLen = (int)log10(number) + 1;
			// neu number ban dau la so am thi numLen + them 1
			numLen += check;

			char* toScreen = new char[numLen + 1];
			toScreen[numLen] = '\0';
			int i = numLen - 1;

			while (number)
			{
				toScreen[i] = (char)((number % 10) + '0');
				number /= 10;
				i--;
			}

			if (check == TRUE)
			{
				toScreen[0] = '-';
			}

			gSynchConsole->Write(toScreen, numLen + 1);

			delete[] toScreen;
			break;
		}

		case SC_readString:
		{
			// goi ham readString
			int virtAd = machine->ReadRegister(4);
			int length = machine->ReadRegister(5);

			char* buffer = new char[length + 1];
			gSynchConsole->Read(buffer, length); // doc tu` ban` phim' len va` nam` trong vung nho systemspace nen khi tra ve thì can` chuyen ve` vung`
												 // nho do vao vung` nho cua nguoi` dung`. Vi char[] buffer tra ve` kieu con tro nen can chuyen
												 // thanh` bo nho' cua user truoc khi tra ve`

			System2User(virtAd, length, buffer);

			delete[] buffer;

			break;
		}

		case SC_printString:
		{
			// goi ham printString
			int virtAd = machine->ReadRegister(4);

			char* buffer = User2System(virtAd, 255);

			int length = 0;
			while (buffer[length] != '\0')
			{
				length++;
			}

			gSynchConsole->Write(buffer, length + 1);

			break;
		}

		case SC_CreateFile:
		{
			int virAddr;
			char* filename;
			DEBUG('a', "\n SC_CreateFile call ......");
			DEBUG('a', "\n Reading virtual address of filename");

			virAddr = machine->ReadRegister(4);
			DEBUG('a', "\n Reading file name");

			filename = User2System(virAddr, MaxFileLength + 1);
			if (strlen(filename) == 0)
			{
				printf("\n File name is not valid");
				DEBUG('a', "\n File name is not valid");
				machine->WriteRegister(2, -1);
				break;
			}

			if (filename == NULL)
			{
				printf("\n Not enough memory in system");
				DEBUG('a', "\n Not enough memory in system");
				machine->WriteRegister(2, -1);
				delete filename;
				break;
			}
			DEBUG('a', "\n Finish reading filename");

			if (!fileSystem->Create(filename, 0))
			{
				printf("\n Error create file '%s'", filename);
				machine->WriteRegister(2, -1);
				delete filename;
				break;
			}

			machine->WriteRegister(2, 0);
			delete filename;
			break;

		}

		case SC_Open:
		{
			//OpenFileId Open(char *name, int type);
			int virtAddr = machine->ReadRegister(4);
			int type = machine->ReadRegister(5);
			char* filename;
			filename = User2System(virtAddr, MaxFileLength);

			int freeSlot = fileSystem->FindFreeSlot();
			if (freeSlot != -1)
			{
				if (type == 0 || type == 1)
				{

					if ((fileSystem->openf[freeSlot] = fileSystem->Open(filename, type)) != NULL)
					{
						machine->WriteRegister(2, freeSlot);
					}
				}
				else if (type == 2)
				{
					machine->WriteRegister(2, 0);
				}
				else
				{
					machine->WriteRegister(2, 1);
				}
				delete[] filename;
				break;
			}
			machine->WriteRegister(2, -1);

			delete[] filename;
			break;
		}

		case SC_Close:
		{
			int fid = machine->ReadRegister(4);
			if (fid >= 0 && fid <= 9)
			{
				if (fileSystem->openf[fid])
				{
					delete fileSystem->openf[fid];
					fileSystem->openf[fid] = NULL;
					machine->WriteRegister(2, 0);
					break;
				}
			}
			machine->WriteRegister(2, -1);
			break;
		}

		case SC_Read:
		{
			int virtAddr = machine->ReadRegister(4); // Lay dia chi cua tham so buffer tu thanh ghi so 4
			int charcount = machine->ReadRegister(5); // Lay charcount tu thanh ghi so 5
			int id = machine->ReadRegister(6); // Lay id cua file tu thanh ghi so 6 
			int OldPos;
			int NewPos;
			char* buf;

			if (id < 0 || id > 9)
			{
				printf("\nKhong the read vi id nam ngoai bang mo ta file.");
				machine->WriteRegister(2, -1);
				IncreasePC();
				return;
			}

			if (fileSystem->openf[id] == NULL)
			{
				printf("\nKhong the read vi file nay khong ton tai.");
				machine->WriteRegister(2, -1);
				IncreasePC();
				return;
			}
			if (fileSystem->openf[id]->type == 3)
			{
				printf("\nKhong the read file stdout.");
				machine->WriteRegister(2, -1);
				IncreasePC();
				return;
			}
			OldPos = fileSystem->openf[id]->GetCurrentPos();
			buf = User2System(virtAddr, charcount);

			if (fileSystem->openf[id]->type == 2)
			{

				int size = gSynchConsole->Read(buf, charcount);
				System2User(virtAddr, size, buf);
				machine->WriteRegister(2, size);
				delete buf;
				IncreasePC();
				return;
			}

			if ((fileSystem->openf[id]->Read(buf, charcount)) > 0)
			{

				NewPos = fileSystem->openf[id]->GetCurrentPos();

				System2User(virtAddr, NewPos - OldPos, buf);
				machine->WriteRegister(2, NewPos - OldPos);
			}
			else
			{
				//printf("\nDoc file rong.");
				machine->WriteRegister(2, -2);
			}
			delete buf;
			IncreasePC();
			return;
		}

		case SC_Write:
		{
			int virtAddr = machine->ReadRegister(4); // Lay dia chi cua tham so buffer tu thanh ghi so 4
			int charcount = machine->ReadRegister(5); // Lay charcount tu thanh ghi so 5
			int id = machine->ReadRegister(6); // Lay id cua file tu thanh ghi so 6
			int OldPos;
			int NewPos;
			char* buf;

			if (id < 0 || id > 9)
			{
				printf("\nKhong the write vi id nam ngoai bang mo ta file.");
				machine->WriteRegister(2, -1);
				IncreasePC();
				return;
			}

			if (fileSystem->openf[id] == NULL)
			{
				printf("\nKhong the write vi file nay khong ton tai.");
				machine->WriteRegister(2, -1);
				IncreasePC();
				return;
			}

			if (fileSystem->openf[id]->type == 1 || fileSystem->openf[id]->type == 2)
			{
				printf("\nKhong the write file stdin hoac file only read.");
				machine->WriteRegister(2, -1);
				IncreasePC();
				return;
			}
			OldPos = fileSystem->openf[id]->GetCurrentPos();
			buf = User2System(virtAddr, charcount);

			if (fileSystem->openf[id]->type == 0)
			{
				if ((fileSystem->openf[id]->Write(buf, charcount)) > 0)
				{

					NewPos = fileSystem->openf[id]->GetCurrentPos();
					machine->WriteRegister(2, NewPos - OldPos);
					delete buf;
					IncreasePC();
					return;
				}
			}
			if (fileSystem->openf[id]->type == 3)
			{
				int i = 0;
				while (buf[i] != 0 && buf[i] != '\n')
				{
					gSynchConsole->Write(buf + i, 1);
					i++;
				}
				buf[i] = '\n';
				gSynchConsole->Write(buf + i, 1);
				machine->WriteRegister(2, i - 1);
				delete buf;
				IncreasePC();
				return;
			}
		}

		case SC_Seek:
		{
			// Input: Vi tri(int), id cua file(OpenFileID)
			// Output: -1: Loi, Vi tri thuc su: Thanh cong
			// Cong dung: Di chuyen con tro den vi tri thich hop trong file voi tham so la vi tri can chuyen va id cua file
			int pos = machine->ReadRegister(4); // Lay vi tri can chuyen con tro den trong file
			int id = machine->ReadRegister(5); // Lay id cua file
			// Kiem tra id cua file truyen vao co nam ngoai bang mo ta file khong
			if (id < 0 || id > 14)
			{
				printf("\nKhong the seek vi id nam ngoai bang mo ta file.");
				machine->WriteRegister(2, -1);
				IncreasePC();
				return;
			}
			// Kiem tra file co ton tai khong
			if (fileSystem->openf[id] == NULL)
			{
				printf("\nKhong the seek vi file nay khong ton tai.");
				machine->WriteRegister(2, -1);
				IncreasePC();
				return;
			}
			// Kiem tra co goi Seek tren console khong
			if (id == 0 || id == 1)
			{
				printf("\nKhong the seek tren file console.");
				machine->WriteRegister(2, -1);
				IncreasePC();
				return;
			}
			// Neu pos = -1 thi gan pos = Length nguoc lai thi giu nguyen pos
			pos = (pos == -1) ? fileSystem->openf[id]->Length() : pos;
			if (pos > fileSystem->openf[id]->Length() || pos < 0) // Kiem tra lai vi tri pos co hop le khong
			{
				printf("\nKhong the seek file den vi tri nay.");
				machine->WriteRegister(2, -1);
			}
			else
			{
				// Neu hop le thi tra ve vi tri di chuyen thuc su trong file
				fileSystem->openf[id]->Seek(pos);
				machine->WriteRegister(2, pos);
			}
			IncreasePC();
			return;
		}
		case SC_Exec:
		{
			// Input: vi tri int
			// Output: Fail return -1, Success: return id cua thread dang chay
			// SpaceId Exec(char *name);
			int virtAddr;
			virtAddr = machine->ReadRegister(4);	// doc dia chi ten chuong trinh tu thanh ghi r4
			char* name;
			name = User2System(virtAddr, MaxFileLength + 1); // Lay ten chuong trinh, nap vao kernel

			if (name == NULL)
			{
				DEBUG('a', "\n Not enough memory in System");
				printf("\n Not enough memory in System");
				machine->WriteRegister(2, -1);
				//IncreasePC();
				return;
			}
			OpenFile* oFile = fileSystem->Open(name);
			if (oFile == NULL)
			{
				printf("\nExec:: Can't open this file.");
				machine->WriteRegister(2, -1);
				IncreasePC();
				return;
			}

			delete oFile;

			// Return child process id
			int id = pTab->ExecUpdate(name);
			machine->WriteRegister(2, id);

			delete[] name;
			IncreasePC();
			return;
		}
		case SC_Join:
		{
			// int Join(SpaceId id)
			// Input: id dia chi cua thread
			// Output: 
			int id = machine->ReadRegister(4);

			int res = pTab->JoinUpdate(id);

			machine->WriteRegister(2, res);
			IncreasePC();
			return;
		}
		case SC_Exit:
		{
			//void Exit(int status);
			// Input: status code
			int exitStatus = machine->ReadRegister(4);

			if (exitStatus != 0)
			{
				IncreasePC();
				return;

			}

			int res = pTab->ExitUpdate(exitStatus);
			//machine->WriteRegister(2, res);

			currentThread->FreeSpace();
			currentThread->Finish();
			IncreasePC();
			return;

		}
		case SC_CreateSemaphore:
		{
			// int CreateSemaphore(char* name, int semval).
			int virtAddr = machine->ReadRegister(4);
			int semval = machine->ReadRegister(5);

			char* name = User2System(virtAddr, MaxFileLength + 1);
			if (name == NULL)
			{
				DEBUG('a', "\n Not enough memory in System");
				printf("\n Not enough memory in System");
				machine->WriteRegister(2, -1);
				delete[] name;
				IncreasePC();
				return;
			}

			int res = semTab->Create(name, semval);

			if (res == -1)
			{
				DEBUG('a', "\n Khong the khoi tao semaphore");
				printf("\n Khong the khoi tao semaphore");
				machine->WriteRegister(2, -1);
				delete[] name;
				IncreasePC();
				return;
			}

			delete[] name;
			machine->WriteRegister(2, res);
			IncreasePC();
			return;
		}

		case SC_Wait:
		{
			// int Wait(char* name)
			int virtAddr = machine->ReadRegister(4);

			char* name = User2System(virtAddr, MaxFileLength + 1);
			if (name == NULL)
			{
				DEBUG('a', "\n Not enough memory in System");
				printf("\n Not enough memory in System");
				machine->WriteRegister(2, -1);
				delete[] name;
				IncreasePC();
				return;
			}

			int res = semTab->Wait(name);

			if (res == -1)
			{
				DEBUG('a', "\n Khong ton tai ten semaphore nay!");
				printf("\n Khong ton tai ten semaphore nay!");
				machine->WriteRegister(2, -1);
				delete[] name;
				IncreasePC();
				return;
			}

			delete[] name;
			machine->WriteRegister(2, res);
			IncreasePC();
			return;
		}
		case SC_Signal:
		{
			// int Signal(char* name)
			int virtAddr = machine->ReadRegister(4);

			char* name = User2System(virtAddr, MaxFileLength + 1);
			if (name == NULL)
			{
				DEBUG('a', "\n Not enough memory in System");
				printf("\n Not enough memory in System");
				machine->WriteRegister(2, -1);
				delete[] name;
				IncreasePC();
				return;
			}

			int res = semTab->Signal(name);

			if (res == -1)
			{
				DEBUG('a', "\n Khong ton tai ten semaphore nay!");
				printf("\n Khong ton tai ten semaphore nay!");
				machine->WriteRegister(2, -1);
				delete[] name;
				IncreasePC();
				return;
			}

			delete[] name;
			machine->WriteRegister(2, res);
			IncreasePC();
			return;
		}
		case SC_Sum:
		{
			// int Sum(int a, int b)
			int a = machine->ReadRegister(4);
			int b = machine->ReadRegister(5);
			int sum = a + b;
			machine->WriteRegister(2, sum);
			IncreasePC();
			return;
		}

		default:
			printf("Unexpected user mode exception %d %d\n", which, type);
			ASSERT(FALSE);
			break;
		}
		IncreasePC();
		break;
	}

	default:
		break;
	}
}