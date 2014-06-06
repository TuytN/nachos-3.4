#include "syscall.h"
#include "copyright.h"
int main()
{
	int i = 32;

	PrintString("Bang ma ASCII:\n");
	for (; i<127; i++)
	{
		PrintInt(i); PrintChar('\t'); PrintChar(i);
		PrintChar('\n');
	}
}
