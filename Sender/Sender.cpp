#include <stdio.h>
#include <Windows.h>
#include <conio.h>

const int MAX_MESSAGE_SIZE = 20;




int main(int argc, char* argv[])
{
	const char* mutexName = "accessMutex";
	const char* readSemName = "readSem";
	const char* writeSemName = "writeSem";
	const char* fifoSemName = "fifoSem";
	const char* binFileName = argv[1];

	int id = atoi(argv[3]);
	int maxRecords = atoi(argv[2]);
	

	char stEvName[30] = "";
	sprintf_s(stEvName, "Start %d", id);

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

	HANDLE hReadSemaphore = OpenSemaphoreA(
		SEMAPHORE_ALL_ACCESS,
		true,
		readSemName
	);

	HANDLE hWriteSemaphore = OpenSemaphoreA(
		SEMAPHORE_ALL_ACCESS,
		true,
		writeSemName
	);

	HANDLE hFifoSemaphore = OpenSemaphoreA(
		SEMAPHORE_ALL_ACCESS,
		true,
		fifoSemName
	);
	
	if (hStartEv) {
		SetEvent(hStartEv);
	}
	else {
		printf("error opening startEvent");
	}

	char choice = 'r';
	char message[MAX_MESSAGE_SIZE + 1] = "";
	long currFifoCount = 0;
	long currPosition = 0;
	FILE* fin;
	errno_t err_fin;


	while (choice == 'r') {
		printf("To read a message, press R key \nTo exit, press any other key      ");
		choice = _getch();
		printf("%c\n", choice);

		if (choice == 'r')
		{
			WaitForSingleObject(hWriteSemaphore, INFINITE);
			WaitForSingleObject(hMutex, INFINITE);

			bool canRelease = ReleaseSemaphore(hFifoSemaphore, 1, &currFifoCount);	//fifoSemaphore is used as a variable indicating
			if (!canRelease)														//where the next message is to be written in file.
			{																		//When maximum amount of records is reached, the 
				currFifoCount = 0;													//semaphore count is dropped back to 0 and 
				for (int i = 0; i < maxRecords - 1; i++) {							//next messages override the first ones in the file
					WaitForSingleObject(hFifoSemaphore, 0);
				}
			}
			
			currPosition = currFifoCount * MAX_MESSAGE_SIZE;
			printf("pos: %d\n", currPosition);

			err_fin = fopen_s(&fin, binFileName, "r+b");

			printf("Enter message: ");
			scanf_s("%s", message, MAX_MESSAGE_SIZE);

			fseek(fin, currPosition, SEEK_SET);
			fwrite(message, MAX_MESSAGE_SIZE, sizeof(char), fin);
			fclose(fin);

			ReleaseMutex(hMutex);
			ReleaseSemaphore(hReadSemaphore, 1, nullptr);
			
		}
	}
}