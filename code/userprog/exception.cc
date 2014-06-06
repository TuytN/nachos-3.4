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

#define MAX_INT_LENGTH 50
#define MAX_STRING_LEN 256
#define LIMIT 50

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

#define MAX_FILE_HANDLER 10
#include "string.h"

void
ExceptionHandler(ExceptionType which)
{
	int type = machine->ReadRegister(2);

	switch (which)
	{
		case SyscallException:
			switch (type)
			{
				case SC_Halt:
					DEBUG('a', "Shutdown, initiated by user program.\n");
					interrupt->Halt();
					break;
				case SC_ReadInt:
				{
				/*Read integer number and return it to $2*/
					DEBUG('a', "Read integer number from console.\n");
					int number = 0; //so se tra ve
					int nBufer = 0; //do dai chuoi doc duoc
					int i;
					char* bufer = new char[50]; //chuoi max 50 ky tu
					nBufer = gSynchConsole->Read(bufer, 50);
					i = bufer[0] == '-' ? 1:0 ; //neu ky tu dau tien la '-' thi bat dau chuyen chuoi sang so tu ky tu thu 1
					for (; i < nBufer; i++) //lap de chuyen ky tu thanh so
					{
						number = number*10 + ((int) (bufer[i]) - 48);
						if((int)bufer[i] > 57 || (int) bufer[i] < 48) //neu ko phai chu so thi number = 0
						{
							number = 0;
							break;
						}
					}

					if(bufer[0] == '-') //doi dau
					{
						number = -1*number;
					}
					else{
						if(bufer[0] > 57 || bufer[0] < 48)
						{
							number = 0;
						}
					}
					machine->WriteRegister(2, number); //ghi ket qua vao thanh ghi $2
					delete bufer;
				}
				break;
				case SC_PrintInt:
				{					
					DEBUG('a', "Read argument value at r4");
					char s[50], neg, temp;
					neg = '-';
					int i, number, mid, sz;
					i = number = 0;
					number = machine->ReadRegister(4);
					if (number < 0)
					{
						gSynchConsole->Write(&neg,1); //ghi dau tru truoc
						number = -number; //chuyen thanh so duong de chuyen doi
					}
					do {
						s[i++] = number%10 + 48;
					} while (( number /= 10) > 0); //lap den khi n van con ky tu
					//sau khi chuyen, s la chuoi nguoc					
					
					sz = i;
					
					s[sz] = '\n';
					
					for (i = 0; i <= (sz-1)/2; i++)
					{
						temp = s[i];
						s[i] = s[sz - i - 1];
						s[sz - i - 1] = temp;
					}
					//mid = i / 2;
					//while (i-->mid)
					//{
					//	tmp = s[sz-i-1];
					//	s[sz-i-1] = s[i];
					//	s[i] = tmp;
					//}
					gSynchConsole->Write(s, sz);
					
				}
				break;
				case SC_ReadString:
				{
					int virtAddr = machine->ReadRegister(4); // doc dia chi chuoi tu thanh ghi so 4
	
				    	int length = machine->ReadRegister(5); // doc chieu dai chuoi tu thanh ghi so 5
					     
					char* buffer = new char[length+1]; 
					    
					int nNumChar = gSynchConsole->Read(buffer, length); // doc chuoi tu console
					 
					int value = machine->System2User(virtAddr, nNumChar+1, buffer); // chuyen vung nho sang user space

					machine->WriteRegister(2, value); 
				}
				break;

				case SC_PrintString:
				{
					int virtAddr = machine->ReadRegister(4);  // doc dia chi vung nho chuoi can xuat tu thanh ghi so 4
	
					char* buffer = new char[MAX_STRING_LEN+1];
					    
					buffer = machine->User2System(virtAddr, MAX_STRING_LEN); // chuyen vung nho tu user space sang kernel space (system)
					    
					gSynchConsole->Write(buffer, MAX_STRING_LEN); // xuat chuoi ra man hinh console	
				}
				break;
				case SC_PrintChar:
				{
					char ch;
					ch = (char) machine->ReadRegister(4);//lay du lieu tu thanh ghi 4 va ep kieu char
					//printf("ch = %c",ch);
					gSynchConsole->Write(&ch, 1);// dua len man hinh console
					break;
				}
				case SC_ReadChar:
				{
					int sz;
					char buf[MAX_INT_LENGTH];
					sz = gSynchConsole->Read(buf,MAX_INT_LENGTH);//so ki tu nguoi dung nhap
					//printf("sz = %d",sz);
					machine->WriteRegister(2, buf[sz - 1]);//lay ki tu cuoi cung va ghi vao thanh ghi so 2
				}
				break;
				case SC_CreateFile:
				{
					int bufAddr = machine->ReadRegister(4); // read string pointer from user
					char *buf = new char[MAX_STRING_LEN]; // mang de luu ten file
					buf = machine->User2System(bufAddr, MAX_STRING_LEN); // doc ten file va luu vao buf
					// create a new file 
					if (fileSystem->Create(buf, 0) == false)// kiem tra xem buf co luu gi ko ? neu ko thi ko tao file
					{
						DEBUG('f',"can not create file");
						machine->WriteRegister(2, -1);
					} else 
					{
						DEBUG('f',"create file successfully");
						machine->WriteRegister(2, 0);
					};
					delete [] buf;
					break;
				}
				case SC_OpenFileID:
				{
					int bufAddr = machine->ReadRegister(4); // read string pointer from user
					int type = machine->ReadRegister(5);
					char *buf = new char[MAX_STRING_LEN];
					if (fileSystem->index > 10)
					{
						machine->WriteRegister(2, -1);
						delete[] buf;
						break;
					}
					buf = machine->User2System(bufAddr, MAX_STRING_LEN);
					if (strcmp(buf,"stdin") == 0)
					{
						printf("stdin mode\n");
						machine->WriteRegister(2, 0);
						break;
					}
					if (strcmp(buf,"stdout") == 0)
					{
						printf("stdout mode\n");
						machine->WriteRegister(2, 1);
						break;
					}
					if ((fileSystem->openf[fileSystem->index] = fileSystem->Open(buf, type)) != NULL)
					{
						DEBUG('f',"open file successfully");
						machine->WriteRegister(2, fileSystem->index-1);
					} else 
					{
						DEBUG('f',"can not open file");
						machine->WriteRegister(2, -1);
					};
					delete [] buf;
					break;
				}
				case SC_CloseFile:
				{
					int m_index = machine->ReadRegister(4); // doc chi so tep tu thanh ghi 
					if (fileSystem->openf[m_index] == NULL) break; //neu tep ko mo thi ngung
					delete fileSystem->openf[m_index]; //neu dang mo thi xoa di chi so tep trong bang
					fileSystem->openf[m_index] = NULL; // gan lai null
					break;
				}
				case SC_ReadFile:
				{
					int bufAddr = machine->ReadRegister(4);
					int NumBuf = machine->ReadRegister(5);
					int m_index = machine->ReadRegister(6);	
					int OldPos;
					int NewPos;
					char *buf = new char[NumBuf];
					int i = 0;
					// Check m_index
					if (m_index < 0 || m_index > 10)
					{
						machine->WriteRegister(2, -1);
						delete[] buf;
						break;
					}
					// check openf[m_index]
					if (fileSystem->openf[m_index] == NULL)
					{
						machine->WriteRegister(2, -1);
						delete[] buf;
						break;
					}
					OldPos = fileSystem->openf[m_index]->GetCurrentPos();
					buf = machine->User2System(bufAddr, NumBuf);
					if (fileSystem->openf[m_index]->type == 2)
					{
						/*  printf("NumBuf = %d\n", NumBuf);*/
						int sz = gSynchConsole->Read(buf, NumBuf);
						/*  machine->System2User(bufAddr, sz, buf);*/
					
						machine->System2User(bufAddr, sz, buf);
						machine->WriteRegister(2, sz);
						break;
					}
					
					if ((fileSystem->openf[m_index]->Read(buf, NumBuf) ) > 0)
					{
						// Copy data from kernel to user space
					  NewPos = fileSystem->openf[m_index]->GetCurrentPos();
						machine->System2User(bufAddr, NewPos - OldPos +1, buf);
						machine->WriteRegister(2, NewPos - OldPos + 1);
					}
					else
					{
						machine->WriteRegister(2, -1);
						delete[] buf;
						break;
					}
					// read data from console 
					
					/*  
					if (fileOpen.type == 2)
					{
						int sz = gSynchConsole->Read(buf, NumBuf);
						machine->System2User(bufAddr, sz, buf);
						machine->WriteRegister(2, sz);
					}*/
					delete[] buf;
					break;
				}
				case SC_WriteFile:
				{
					int bufAddr = machine->ReadRegister(4);
					int NumBuf = machine->ReadRegister(5);
					int m_index =  machine->ReadRegister(6);
					int OldPos;
					int NewPos;
					char *buf = new char[NumBuf];
					// Check m_index
					if (m_index < 0 || m_index > 10)
					{
						machine->WriteRegister(2, -1);
						delete[] buf;
						break;
					}
					// check openf[m_index]
					if (fileSystem->openf[m_index] == NULL)
					{
						machine->WriteRegister(2, -1);
						delete[] buf;
						break;
					}
					OldPos = fileSystem->openf[m_index]->GetCurrentPos();
					
					// type must equals '0'
					buf = machine->User2System(bufAddr, NumBuf);
					if (fileSystem->openf[m_index]->type  == 0 || fileSystem->openf[m_index]->type == 3)
					{	
					if ((fileSystem->openf[m_index]->Write(buf, NumBuf)) > 0) 
					{
						// Copy data from kernel to user space
						printf("%s",buf);
						NewPos = fileSystem->openf[m_index]->GetCurrentPos();
						machine->WriteRegister(2, NewPos - OldPos + 1);
					}
					else if (fileSystem->openf[m_index]->type == 1);
					{
						machine->WriteRegister(2, -1);
						delete[] buf;
						break;
					}
					}
					// Write data to console
					if (fileSystem->openf[m_index]->type == 3)
					{
						int i = 0;
						printf("stdout mode\n");
						while (buf[i] != 0 && buf[i] != '\n')
						{
							gSynchConsole->Write(buf+i, 1);
							i++;
						}
						buf[i] = '\n';
						gSynchConsole->Write(buf+i,1);
						machine->WriteRegister(2, i-1);
					}
					delete[] buf;
					break;
				}
				case SC_SeekFile:
				{
					int pos = machine->ReadRegister(4);
					int m_index = machine->ReadRegister(5);
					if (m_index < 0 || m_index > 10)
					{
						machine->WriteRegister(2, -1);
						break;
					}
					// check openf[m_index]
					if (fileSystem->openf[m_index] == NULL)
					{
						printf("seek fail \n");
						machine->WriteRegister(2, -1);
						break;
					}
						pos = (pos == -1) ? fileSystem->openf[m_index]->Length() : pos;
					if (pos > fileSystem->openf[m_index]->Length() || pos < 0) {
						machine->WriteRegister(2, -1);
					} else 
					{
						fileSystem->openf[m_index]->Seek(pos);
						machine->WriteRegister(2, pos);
					}
					break;
				}
		 //...
			}
	 	//...

		machine->registers[PrevPCReg] = machine->registers[PCReg]; // for debugging, in case we
		// are jumping into lala-land
		machine->registers[PCReg] = machine->registers[NextPCReg];
		machine->registers[NextPCReg] += 4;
		break;
	case  PageFaultException:    // No valid translation found
		printf("No valid translation found %d %d\n", which, type);
		ASSERT(FALSE);
		break;
	case  ReadOnlyException:     // Write attempted to page marked 
		printf("Write attempted tp page marked %d %d\n", which, type);
		ASSERT(FALSE);
		break;
	case  BusErrorException:     // Translation resulted in an 
		printf("Translaion resulted in an %d %d\n", which, type);
		ASSERT(FALSE);
		break;
	case  AddressErrorException: // Unaligned reference or one that
		printf("Unaligned reference or one that %d %d\n", which, type);
		ASSERT(FALSE);
		break;
	case  OverflowException:     // Integer overflow in add or sub.
		printf("Integer overflow in add or sub %d %d\n", which, type);
		ASSERT(FALSE);
		break;
	case  IllegalInstrException: // Unimplemented or reserved instr.
		printf("Unimplemented or reserved instr %d %d\n", which, type);
		ASSERT(FALSE);
		break;
	}
}
