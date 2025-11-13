/*
 * Name: Zhenjiang Tian (BlazerID: ztian)
 * Lab 11
 * Compile:  gcc -Wall -Wextra -O2 lab11.c -o lab11
 * Run:      ./lab11
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define BUFSIZE 1024

int main(void) {
    char command[BUFSIZE];

    while (1) {
        printf("Enter command: ");
        fflush(stdout);

        if (fgets(command, sizeof(command), stdin) == NULL)
            continue;

        command[strcspn(command, "\n")] = '\0';

        if (strcmp(command, "quit") == 0) {
            printf("Exiting program...bye!\n");
            break;
        }

        FILE *fp = popen(command, "r");
        if (fp == NULL) {
            perror("popen failed");
            continue;
        }

        char buffer[BUFSIZE];
        while (fgets(buffer, sizeof(buffer), fp) != NULL) {
            fputs(buffer, stdout);
        }

        pclose(fp);
    }

    return 0;
}
