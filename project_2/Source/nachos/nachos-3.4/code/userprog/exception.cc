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


// Function to handle run-time errors 
void ExceptionHandler(ExceptionType which)
{
	int type = machine->ReadRegister(2);

	switch (which)
	{
		case NoException:
			return;

		case PageFaultException:
			printf("No valid translation found \n\n");
			ASSERT(FALSE);
			interrupt->Halt();

			break;

		case PageFaultException:
			printf("Write attempted to page marked \"read - only\" \n\n");
			ASSERT(FALSE);
			interrupt->Halt();

			break;

		case BusErrorException:
			printf("Translation resulted in an invalid physical address \n\n");
			ASSERT(FALSE);
			interrupt->Halt();
			break;

		case AddressErrorException:
		
			printf("Unaligned reference or one that was beyond the end of the address space \n \n");
			ASSERT(FALSE);
			interrupt->Halt();
			break;

		case OverflowException:
			
			printf("Integer overflow in add or sub. \n \n");
			ASSERT(FALSE);
			interrupt->Halt();
			break;

		case IllegalInstrException:
			
			printf("Unimplemented or reserved instr. \n\n");
			ASSERT(FALSE);
			interrupt->Halt();
			break;

		case NumExceptionTypes:
			
			printf("Number exception types \n\n");
			ASSERT(FALSE);
			interrupt->Halt();
			break;

			// khi cai cac ham systemcall phai xu li loi va in ra 
		case SyscallException:
			switch (type)
			{
			case SC_Halt:
				printf("\nShutdown, initiated by user program. ");
				interrupt->Halt();
				break;

			case SC_readChar:
				// goi ham readChar
				break;

			case SC_printChar:
				// goi ham printChar
				break;

			case SC_readInt:
				//goi ham readInt
				break;
				
			case SC_printInt:
				//goi ham printInt
				break;

			case SC_readString:
				//goi ham readString
				break;

			case SC_printString:
				// goi ham printString
				break;

			default:
				printf("Unexpected user mode exception %d %d\n", which, type);
				ASSERT(FALSE);
				break;
			}
		default:
			break;
		}
}
