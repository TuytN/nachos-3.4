#include "syscall.h"
#include "copyright.h"
int main()
{
	
	int i = 0, j = 1;
	int temp = 0;
	int n = 0;
	int arr[100]; //max 100 so
	
	PrintString("Nhap N: ");
	n = ReadInt();
	
	for(; i < n; i++)
	{
		PrintString("Nhap a["); PrintInt(i); PrintString("]: ");
		arr[i] = ReadInt();
	}

	i = 0;
	for (; i < n - 1; i++)
	{
		for(j = i + 1; j < n; j++)
		{
			if(arr[i] > arr[j])
			{
				temp = arr[i];
				arr[i] = arr[j];
				arr[j] = temp;	
			}
		}
	}

	i = 0;
	for (; i < n; i++)
	{
		PrintInt(arr[i]);
		PrintString("\t");
	}
}
