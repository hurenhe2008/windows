#include <stdio.h>
#include <Windows.h>
#include "traverse_thread.h"


int main(int argc, char *argv[])
{
    char inputstr[MAX_PATH + 1] = { 0 };
    traverse_data_t data = { MAX_PATH, inputstr };

    do
    {
        inputstr[MAX_PATH] = '\0';

        printf("please input the search folder: \n");
        scanf_s("%s", inputstr, MAX_PATH);

        if ('\0' != inputstr[MAX_PATH]) {
            printf("input search folder too long, again!\n");
            continue;
        }

        printf("press any key to run [press '#' key to stop when run] \n");
        getchar();  /*remove '\n' charater in stdin buffer*/
        getchar();

        if (!traverse_start(&data)) {
            printf("traserve thread error, transerve %s failed!\n", inputstr);
            break;
        }

        getchar();  /*remove '\n' charater in stdin buffer*/
        char choice = 0;
        scanf_s("%c", &choice, sizeof(char));

        if ('#' == choice) {
            if (!traverse_stop()) {
                printf("transerve thread exit error, exit!\n");
                break;
            }
        }

        fflush(stdin);   
        printf("press any key to continue!\n");
        getchar();

        system("cls");
    } while (true);
   
    printf("application exit!\n");
    getchar();

    return 0;
}

