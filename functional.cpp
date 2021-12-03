#include "plugins_manager.h"

void LoadPlugins(int _language_number)
{
    pluginsCount = 0;
    WIN32_FIND_DATA file;
    HANDLE hFile = FindFirstFile("Plugins\\*.dll", &file);

    if (hFile == INVALID_HANDLE_VALUE)
    {
        return;
    }
    do
    {
        if (pluginsCount == MAXPLUGINSALLOWED)
        {
            break;
        }
        char buf[256] = "Plugins\\";
        strcat_s(buf, 256, file.cFileName);
        Plugins[pluginsCount] = LoadLibrary(buf);
        ++pluginsCount;
    } while (FindNextFile(hFile, &file));
    FindClose(hFile);

    if (_language_number == 1)
        printf("Загруженные плагины: %d \n\n", pluginsCount);
    else
        printf("Plugins loaded: %d \n\n", pluginsCount);
}

void UnloadPlugins(int _language_number)
{
    if (pluginsCount == 0)
    {
        if (_language_number == 1)
            printf("%s\n\n", "Нет загруженных плагинов");
        else
            printf("%s\n\n", "There're no loaded plugins");
        return;
    }
    do
    {
        FreeLibrary(Plugins[pluginsCount - 1]);
        Plugins[pluginsCount - 1] = NULL;
        --pluginsCount;
    } while (pluginsCount != 0);

    if (_language_number == 1)
        printf("%s\n\n", "Все плагины были выгружены.");
    else
        printf("%s\n\n", "All plugins were unloaded.");
}

void ShowInfo(int _language_number)
{
    if (pluginsCount == 0)
    {
        if (_language_number == 1)
            printf("%s\n\n", "Нет загруженных плагинов");
        else
            printf("%s\n\n", "There're no loaded plugins");
        return;
    }
    DWORD bufdword = 0;
    char buffer[512];
    PluginInfo PI;
    for (int i = 0; i < pluginsCount; ++i)
    {
        PI = (PluginInfo)GetProcAddress(Plugins[i], "GetDescription");
        PI(buffer, 512, &bufdword);

        if (_language_number == 1)
            printf("Описание плагина%d: %s\n", i + 1, buffer);
        else
            printf("Plugin%d description: %s\n", i + 1, buffer);

        PI = (PluginInfo)GetProcAddress(Plugins[i], "GetAuthor");
        PI(buffer, 512, &bufdword);
        if (_language_number == 1)
            printf("Автор: %s\n\n", buffer);
        else
            printf("Author: %s\n\n", buffer);
    }
}

void RunPlugins(int _language_number)
{
    if (pluginsCount == 0)
    {
        if (_language_number == 1)
            printf("%s\n\n", "Нет загруженных плагинов");
        else
            printf("%s\n\n", "There're no loaded plugins");
        return;
    }
    PluginExecute PE;
    for (int i = 0; i < pluginsCount; ++i)
    {
        PE = (PluginExecute)GetProcAddress(Plugins[i], "Execute");
        PE();
    }
}


void Exit(int _language_number)
{
    if (pluginsCount != 0)
    {
        do
        {
            FreeLibrary(Plugins[pluginsCount - 1]);
            Plugins[pluginsCount - 1] = NULL;
            --pluginsCount;
        } while (pluginsCount != 0);
        if (_language_number == 1)
            printf("%s\n\n", "Все плагины были выгружены");
        else
            printf("%s\n\n", "All plugins were unloaded.");
    }
    if (_language_number == 1)
        printf("%s", "Завершение программы...");
    else
        printf("%s", "Program finished...");
}

