#include "syscall.h"
#include "copyright.h"
int main()
{
	char buf[256];
	PrintString("Nhap 1 chuoi: ");
	ReadString(buf,256);
	PrintString("Chuoi vua nhap la: ");
	PrintString(buf);
}
