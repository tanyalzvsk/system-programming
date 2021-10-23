//Lazovskaya Tatiana Andreevna

#include <windows.h>
#include <tlhelp32.h>
#include  <stdio.h>

int main()
{
	HANDLE hProcessSnapShot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
	PROCESSENTRY32 pe;
	pe.dwSize = sizeof(PROCESSENTRY32);
	if (Process32First(hProcessSnapShot, &pe))
	{
		do
		{
			printf("PID: %d\n", pe.th32ProcessID);
			printf("Parent PID: %d\n", pe.th32ParentProcessID);
			printf("Priority: %d\n", pe.pcPriClassBase);
			printf("Count of threads_area: %d\n", pe.cntThreads);
			int count = 0;
			HANDLE hThreadSnapShot = CreateToolhelp32Snapshot(TH32CS_SNAPTHREAD, pe.th32ProcessID);
			THREADENTRY32 te;
			te.dwSize = sizeof(THREADENTRY32);

			HANDLE hModuleSnapShot = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE, pe.th32ProcessID);
			MODULEENTRY32 me;
			me.dwSize = sizeof(MODULEENTRY32);

			if (Thread32First(hThreadSnapShot, &te))
			{
				do
				{
					if (te.th32OwnerProcessID == pe.th32ProcessID)
					{
						count++;
						printf("\tTID: %d\n", te.th32ThreadID);
						printf("\tParent TID: %d\n", te.th32OwnerProcessID);
						printf("\tPriority of the tread: %d\n", te.tpBasePri);
					}

				} while (Thread32Next(hThreadSnapShot, &te));
			}
			printf("Count of threads: %d\n", count);

			count = 0;
			if (Module32First(hModuleSnapShot, &me))
			{
				do
				{
					count++;
					printf("\tModule size: %d\n", me.modBaseSize);
					printf("\tPath: %s\n", me.szExePath);

				} while (Module32Next(hModuleSnapShot, &me));
			}

			printf("Count of modules: %d\n\n", count);

			CloseHandle(hThreadSnapShot);
			CloseHandle(hModuleSnapShot);

		} while (Process32Next(hProcessSnapShot, &pe));

	}
	CloseHandle(hProcessSnapShot);
	return 0;

}
