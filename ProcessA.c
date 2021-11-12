// Lazovskaya Tatiana Andreevna

#include <windows.h>
#include <stdio.h>
#include <TlHelp32.h>

DWORD GetProcessID(char* process) {
	PROCESSENTRY32 processEntry;													
	DWORD processID = 0;			
	HANDLE Snapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);		    

	if (Snapshot != INVALID_HANDLE_VALUE) {
		processEntry.dwSize = sizeof(processEntry);
		if (Process32First(Snapshot, &processEntry))							 
		{
			while (Process32Next(Snapshot, &processEntry)) {
				if (strcmp(processEntry.szExeFile, process) == 0) {
					processID = processEntry.th32ProcessID;
					break;
				}
			}
		}
		CloseHandle(Snapshot);
	}

	return processID;
}

int main()
{
	LPCTSTR name_of_pipe = "\\\\.\\pipe\\name_of_pipe";	
	HANDLE Handle_of_Pipe = INVALID_HANDLE_VALUE;
	HANDLE Handle_of_ProcessB;												
	LPVOID AddressB;														
	BOOL Checker;														
	DWORD Str_size = sizeof(char) * 1000;						       
	char *str = (char*)malloc(Str_size);								
								

	printf("Enter string: ");
	gets_s(str, 999);													

	printf("Process B is opening");
	Handle_of_ProcessB = OpenProcess(PROCESS_ALL_ACCESS, FALSE, GetProcessID("ProcessB.exe"));
	if (Handle_of_ProcessB == INVALID_HANDLE_VALUE) {
		printf("error!\n");
		system("pause");
		return -1;
	}

	printf("\nAllocate memory for Process B");
	AddressB = VirtualAllocEx(Handle_of_ProcessB, 0, Str_size, MEM_RESERVE | MEM_COMMIT, PAGE_EXECUTE_READWRITE);

	
	printf("\nInputting entered string in ProcessB memory");
	Checker = WriteProcessMemory(Handle_of_ProcessB, AddressB, str, Str_size, 0);
	if (Checker == FALSE) {
		printf("error!\n");
		system("pause");
		return -1;
	}

	printf("\nConnection with named pipe");
	while (Handle_of_Pipe == INVALID_HANDLE_VALUE) {
		Handle_of_Pipe = CreateFile(name_of_pipe, GENERIC_READ | GENERIC_WRITE, 0, 0, OPEN_EXISTING, 0, 0);													
	}

	printf("\nInputting memory address");
	Checker = WriteFile(Handle_of_Pipe, &AddressB, sizeof(AddressB), 0, 0);					
	if (Checker == FALSE) {
		printf("error!\n");
		system("pause");
		return -1;
	}

	printf("\nWait answer from Process B");
	Checker = ReadFile(Handle_of_Pipe, &AddressB, sizeof(AddressB), 0, 0);
	if (Checker == FALSE) {
		printf("error!\n");
		system("pause");
		return -1;
	}
	printf("\nFree Process B memory");
	Checker = VirtualFreeEx(Handle_of_ProcessB, AddressB, 0, MEM_RELEASE);
	if (Checker == FALSE) {
		printf("error!\n");
		system("pause");
		return -1;
	}
	printf("\nClose named pipe");
	Checker = CloseHandle(Handle_of_Pipe);
	if (Checker == FALSE) {
		printf("error!\n");
		system("pause");
		return -1;
	}

	printf("\nClose Process B");
	Checker = CloseHandle(Handle_of_ProcessB);
	if (Checker == FALSE) {
		printf("error!\n");
		system("pause");
		return -1;
	}

	printf("\nProcess A was finished successfully!\n");
	system("pause");
	return 0;
}
