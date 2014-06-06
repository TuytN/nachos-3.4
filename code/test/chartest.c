#include "syscall.h"
#include "copyright.h"
int main()
{
	char ch;
	PrintString("nhap vao 1 ki tu: ");
	ch = ReadChar();
	PrintString("ki tu vua nhap la: ");	
	PrintChar(ch);
}
