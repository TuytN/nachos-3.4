#include "syscall.h"
#include "copyright.h"
int main()
{
	int number;
	PrintString("nhap vao 1 so: ");
	number = ReadInt();
	PrintString("so vua nhap la: ");	
	PrintInt(number);
}
