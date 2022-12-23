#include <stdio.h>
#include <Windows.h>
#include <conio.h>

const int MAX_MESSAGE_SIZE = 20;

int main(int argc, char* argv[])
{
	const char* mutexName = "accessMutex";
	const char* readSemName = "readSem";
	const char* writeSemName = "writeSem";
	const char* binFileName = argv[1];
	int id = atoi(argv[2]);
	

	char stEvName[30] = "";
	sprintf_s(stEvName, "Start %s", argv[2]);

	HANDLE hStartEv = OpenEventA(
		EVENT_ALL_ACCESS,
		true,
		stEvName
	);

	HANDLE hMutex = OpenMutexA(
		MUTEX_ALL_ACCESS,
		true,
		mutexName
	);

	HANDLE hreadSemaphore = OpenSemaphoreA(
		SEMAPHORE_ALL_ACCESS,
		true,
		readSemName
	);

	HANDLE hwriteSemaphore = OpenSemaphoreA(
		SEMAPHORE_ALL_ACCESS,
		true,
		writeSemName
	);
	
	if (hStartEv) {
		SetEvent(hStartEv);
	}
	else {
		printf("error opening startEvent");
	}

	char choice = 'r';
	char message[MAX_MESSAGE_SIZE + 1] = "";
	int currPosition = 0;
	FILE* fin;
	errno_t err_fin;


	while (choice == 'r') {
		printf("To read a message, press R key \nTo exit, press any other key      ");
		choice = _getch();
		printf("%c\n", choice);

		if (choice == 'r')
		{
			err_fin = fopen_s(&fin, binFileName, "ab");

			printf("Enter message: ");
			scanf_s("%s", message, MAX_MESSAGE_SIZE);

			fwrite(message, MAX_MESSAGE_SIZE, sizeof(char), fin);
			fclose(fin);
			
		}
	}


	
	_getch();
}