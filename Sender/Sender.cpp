#include <stdio.h>
#include <Windows.h>
#include <conio.h>

int main(int argc, char* argv[])
{
	int id = atoi(argv[2]);
	char stEvName[30] = "";
	sprintf_s(stEvName, "Start %s", argv[2]);
	HANDLE hStartEv = OpenEventA(
		EVENT_ALL_ACCESS,
		true,
		stEvName
	);
	
	if (hStartEv) {
		SetEvent(hStartEv);
	}
	else {
		printf("error opening startEvent");
	}
	
	_getch();
}