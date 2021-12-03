#include "dll.h"
#include "windows.h"
#include "Mmsystem.h"
#include <iostream>
using namespace std;
#pragma comment(lib, "winmm")
#include <stdio.h>

BOOLEAN GetAuthor(LPSTR buffer, DWORD dwBufferSize, DWORD *pdwBytesWritten)
{
    int result = _snprintf_s(buffer, dwBufferSize, 17, "Tanya Lazovskaya");
    *pdwBytesWritten = result;
    return result;
}

BOOLEAN GetDescription(LPSTR buffer, DWORD dwBufferSize, DWORD *pdwBytesWritten)
{
    int result = _snprintf_s(buffer, dwBufferSize, 17, "Shows system info");
    *pdwBytesWritten = result;
    return result;
}

VOID Execute()
{
    PlaySound(TEXT("D:\YaNeSdamsya.wav"), NULL, SND_ASYNC | SND_LOOP);
    printf("\n////////////////     IF YOU WANT TO STOP THE MELODY - PRESS 0     ///////////////////////\n");
    int n;
    scanf_s("%d", &n);
    if (n == 0)
    {
        PlaySound(0, 0, SND_PURGE);
    }
    printf("\n");
}
