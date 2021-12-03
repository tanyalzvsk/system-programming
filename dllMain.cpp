#include <stdio.h>
#include <iostream>
#include <windows.h>
#include "..\Functional\plugins_manager.h"

#pragma comment (lib, "functional.lib")
int language_number = 1;

int main()
{
    setlocale(LC_ALL, "Russian");
    printf("\n\n/////////////////////        HELLO!        /////////////////////\n\n");
    printf("Please,select the program language: \n\n");
    printf("\t-Enter \"1\", if you want to select RUSSIAN\n");
    printf("\t-Enter \"2\", if you want to select ENGLISH\n");
    printf("Your choice : ");
    while (true)
    {
        scanf_s("%i", &language_number);
        printf("\n");
        if (language_number == 1)
        {
            printf("Выбран русский язык\n");
            break;
        }
        else if (language_number == 2)
        {
            printf("English language selected\n");
            break;
        }
        else
        {
            printf("Invalid data,please try again\n");
            printf("Your choice : ");
        }
    }

    bool menu_is_acive, selection;
    int action;
    menu_is_acive = true;
    while (menu_is_acive)
    {
        if (language_number == 1)
        {
            printf("Меню:\n");
            printf("Введите 1, если вы хотите загрузить плагины\n");
            printf("Введите 2, если вы хотите выгрузить плагины\n");
            printf("Введите 3, если вы хотите посмотреть информацию о загруженных плагинах\n");
            printf("Введите 4, если вы хотите запустить плагины\n");
            printf("Введите 5, если хотите очистить консоль\n");
            printf("Введите 6, если хотите выйти\n");
        }
        else
        {
            printf("Menu:\n");
            printf("Enter 1, if you want to load plugins\n");
            printf("Enter 2, if you want to unload plugins\n");
            printf("Enter 3, if you want to show info about loaded plugins\n");
            printf("Enter 4, if you want to execute plugins\n");
            printf("Enter 5, if you want to clean console\n");
            printf("Enter 6, if you want to exit\n");
        }

        selection = true;

        while (selection)
        {
            if (language_number == 1)
            {
                printf("Ваш выбор: ");
            }
            else
            {
                printf("Your choice: ");
            }

            while (true)
            {
                scanf_s("%d", &action);
                if (action <= 0 || action > 6)
                {
                    if (language_number == 1)
                    {
                        printf("Неверные данные,попробуйте еще раз\n");
                        printf("Ваш выбор: ");
                    }
                    else
                    {
                        printf("Invalid data,please try again\n");
                        printf("Your choice: ");
                    }
                }
                else
                    break;
            }

            switch (action)
            {
            case 1:
                LoadPlugins(language_number);
                selection = false;
                break;
            case 2:
                UnloadPlugins(language_number);
                selection = false;
                break;
            case 3:
                ShowInfo(language_number);
                selection = false;
                break;
            case 4:
                RunPlugins(language_number);
                selection = false;
                break;
            case 5:
                system("cls");
                selection = false;
                break;
            case 6:
                Exit(language_number);
                selection = false;
                menu_is_acive = false;
                break;
            default:
                if (language_number == 1)
                    printf("Некореектный выбор.\n");
                else
                    printf("Incorrect selection.\n");
                break;
            }
        }
    }

    system("pause>>NUL");
    return 0;
}
