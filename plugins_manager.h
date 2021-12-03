#pragma once

#include <stdio.h>
#include <windows.h>

#define  MAXPLUGINSALLOWED 10

#ifdef __cplusplus

extern "C" 
{
#endif

	typedef void(*PluginExecute)();
	typedef bool(*PluginInfo)(LPSTR, DWORD, DWORD*);
	__declspec(dllexport) HINSTANCE Plugins[MAXPLUGINSALLOWED];
	__declspec(dllexport) int pluginsCount;
	__declspec(dllexport) void LoadPlugins(int _language_number);
	__declspec(dllexport) void UnloadPlugins(int _language_number);
	__declspec(dllexport) void ShowInfo(int _language_number);
	__declspec(dllexport) void RunPlugins(int _language_number);
	__declspec(dllexport) void Exit(int _language_number);

#ifdef __cplusplus
};
#endif
