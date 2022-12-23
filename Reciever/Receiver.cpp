#include <stdio.h>
#include <Windows.h>
#include <iostream>
#include <conio.h>

const int MAX_BINFILE_NAME = 50;
const int NAME_SIZE = 30;
const int MAX_MESSAGE_SIZE = 20;

int main()
{
	char binFileName[MAX_BINFILE_NAME] = "";
	char lpCommandLineTemplate[100] = "Sender.exe";
	char stEvNameTemp[NAME_SIZE] = "";
	const char* mutexName = "accessMutex";
	const char* readSemName = "readSem";
	const char* writeSemName = "writeSem";
	int maxRecords = 0;
	int numOfSenders = 0;

	printf("Enter the name of the binary file (without extension): ");
	scanf_s("%s", binFileName, MAX_BINFILE_NAME);
	strcat_s(binFileName, ".bin");

	printf("Enter the maximum number of records in the file: ");
	scanf_s("%d", &maxRecords);
	
	printf("Enter the number of Sender processes: ");
	scanf_s("%d", &numOfSenders);

	strcat_s(lpCommandLineTemplate, " ");
	strcat_s(lpCommandLineTemplate, binFileName);

	FILE* fin;
	errno_t err_fin = fopen_s(&fin, binFileName, "w+b");
	fclose(fin);

	HANDLE hMutex = CreateMutexA(
		nullptr,
		true,
		mutexName
	);

	HANDLE hReadSemaphore = CreateSemaphoreA(
		nullptr,
		0,
		maxRecords,
		readSemName
	);

	HANDLE hWriteSemaphore = CreateSemaphoreA(
		nullptr,
		maxRecords,
		maxRecords,
		writeSemName
	);

	HANDLE* hSenders = new HANDLE[numOfSenders];
	HANDLE* hStartEvents = new HANDLE[numOfSenders];

	STARTUPINFOA* si = new STARTUPINFOA[numOfSenders];
	PROCESS_INFORMATION* pi = new PROCESS_INFORMATION[numOfSenders];
	for (int i = 0; i < numOfSenders; i++) {
		ZeroMemory(&si[i], sizeof si[i]);
		si[i].cb = sizeof si[i];
		ZeroMemory(&pi[i], sizeof pi);
	}

	for (int i = 0; i < numOfSenders; i++) {
		sprintf_s(stEvNameTemp, "Start %d", i);
		hStartEvents[i] = CreateEventA(
			nullptr,
			false,
			false,
			stEvNameTemp
		);
	}

	char lpCommandLine[100];
	for (int i = 0; i < numOfSenders; i++) {
		sprintf_s(lpCommandLine, "%s %d", lpCommandLineTemplate, i);
		printf("%s\n", lpCommandLine);

		CreateProcessA(
			nullptr,
			lpCommandLine,
			nullptr,
			nullptr,
			true,
			CREATE_NEW_CONSOLE,
			nullptr,
			nullptr,
			&si[i],
			&pi[i]
		);
	}

	WaitForMultipleObjects(numOfSenders, hStartEvents, true, 5000);

	char choice = 'r';
	char message[MAX_MESSAGE_SIZE + 1] = "";
	int currPosition = 0;

	
	while (choice == 'r') {
		printf("To read a message, press R key \nTo exit, press any other key      ");
		choice = _getch();
		printf("%c\n", choice);

		if (choice == 'r') 
		{
			err_fin = fopen_s(&fin, binFileName, "rb");
			if (ftell(fin) != SEEK_END)
			{
				fseek(fin, 0, currPosition);
				fread(message, MAX_MESSAGE_SIZE, sizeof(char), fin);
				printf("Message: %s \n", message);
				currPosition += MAX_MESSAGE_SIZE;
			}
			else
			{
				printf("End of file\n");
			}
			fclose(fin);
		}
	}
}