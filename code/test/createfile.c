/* createfile syscall demo */

#include "syscall.h"

#include "copyright.h"



int main(int argc, char* argv[])

{

	int isCreate = CreateFile("hello");

	char mess[255];

	OpenFileID open; 

	if (isCreate)

	{

		PrintString("Can not create file \'hello.txt\'\n");

	} 

	else

	{

		PrintString("Tao file thanh cong\n");
		PrintString("dang mo file...\n");

		open = OpenFileFunc("hello", 0);
		
		if (open==-1) 
			PrintString("Ko the mo file\n");
		else PrintString("file da mo\n");
		
		//PrintString("Viet vao file: ");
		//ReadString(mess,256);
		
		SeekFile(0, open);
		WriteFile("Hello NachOS", 12, open);

		SeekFile(0, open);

		PrintString("noi dung da ghi vao file: ");
		ReadFile(mess, 12, open);
		PrintString(mess);
		PrintString("\n");

		CloseFile(open);

	}
	
	return 0;
}
