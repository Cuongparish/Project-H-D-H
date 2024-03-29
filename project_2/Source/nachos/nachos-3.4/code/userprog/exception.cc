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
char *User2System(int virtAd, int limit)
{
	int n; // chi so index
	int oneChar;
	char *kernelBuf = NULL;
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
int System2User(int virtAd, int len, char *buffer)
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

			char *buffer = new char[255];
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
			char *buffer = new char[255];
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

			char *toScreen = new char[numLen + 1];
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

			char *buffer = new char[length + 1];
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

			char *buffer = User2System(virtAd, 255);

			int length = 0;
			while (buffer[length] != '\0')
			{
				length++;
			}

			gSynchConsole->Write(buffer, length + 1);

			break;
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
