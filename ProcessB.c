// Lazovskaya Tatiana Andreevna

#include <windows.h>
#include <stdio.h>


int main(void) {
	LPCTSTR name_of_pipe = "\\\\.\\pipe\\name_of_pipe";					
	HANDLE  Handle_of_Named_Pipe;											
	DWORD Allocated_memory_address;										
	char* str;															
	BOOL Checker;														

	printf("Pipe creation");
	Handle_of_Named_Pipe = CreateNamedPipe(name_of_pipe,
										PIPE_ACCESS_DUPLEX,
										PIPE_TYPE_MESSAGE |
										PIPE_READMODE_MESSAGE |
										PIPE_WAIT,
										PIPE_UNLIMITED_INSTANCES,
										sizeof(DWORD),
										sizeof(DWORD),
										100,
										0);
										
	if (Handle_of_Named_Pipe == INVALID_HANDLE_VALUE) {
		printf("error!\n");
		system("pause");
		return -1;
	}

	printf("\nPipe connection");
	ConnectNamedPipe(Handle_of_Named_Pipe, 0);

	printf("\nRead from pipe");
	Checker = ReadFile(Handle_of_Named_Pipe, &Allocated_memory_address, sizeof(Allocated_memory_address), 0, 0);
	if (Checker == FALSE) {
		printf("error!\n");
		system("pause");
		return -1;
	}
	printf("\nEntered string: ");
	str = (char*)Allocated_memory_address;
	printf_s("%s\n", str);


	printf("Direct answer to Proccess A");
	Checker = WriteFile(Handle_of_Named_Pipe, &Allocated_memory_address, sizeof(Allocated_memory_address), 0, 0);
	if (Checker == FALSE) {
		printf("error!\n");
		system("pause");
		return -1;
	}

	printf("\nDisconnect from named pipe");
	Checker = DisconnectNamedPipe(Handle_of_Named_Pipe);
	if (Checker == FALSE) {
		printf("error!\n");
		system("pause");
		return -1;
	}

	printf("\nClose named pipe");
	Checker = CloseHandle(Handle_of_Named_Pipe);
	if (Checker == FALSE) {
		printf("error!\n");
		system("pause");
		return -1;
	}
	printf("\nProcess B was finished successfully!\n");
	system("pause");
	return 0;
}