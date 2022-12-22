#include <stdio.h>
#include <Windows.h>
#include <iostream>

const int MAX_BINFILE_NAME = 50;

int main()
{
	char binFileName[MAX_BINFILE_NAME] = "";
	int maxRecords;
	int numOfSenders;

	printf("Enter the name of the binary file: ");
	scanf_s("%s", binFileName, MAX_BINFILE_NAME);

	printf("Enter the maximum number of records in the file: ");
	scanf_s("%d", &maxRecords);
	
	printf("Enter the number of Sender processes: ");
	scanf_s("%d", &numOfSenders);

	HANDLE* hSenders = new HANDLE[numOfSenders];
	HANDLE* hStartEvents = new HANDLE[numOfSenders];

	STARTUPINFOA* si = new STARTUPINFOA[numOfSenders];
	PROCESS_INFORMATION* pi = new PROCESS_INFORMATION[numOfSenders];
	for (int i = 0; i < numOfSenders; i++) {
		ZeroMemory(&si[i], sizeof si[i]);
		si[i].cb = sizeof si[i];
		ZeroMemory(&pi[i], sizeof pi);
	}


	char stEvNameTemp[30] = "";
	for (int i = 0; i < numOfSenders; i++) {
		sprintf_s(stEvNameTemp, "Start %d", i);
		hStartEvents[i] = CreateEventA(
			nullptr,
			false,
			false,
			stEvNameTemp
		);
	}

	char lpCommandLineTemplate[100] = "Sender.exe";
	strcat_s(lpCommandLineTemplate, " ");
	strcat_s(lpCommandLineTemplate, binFileName);

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

	
}