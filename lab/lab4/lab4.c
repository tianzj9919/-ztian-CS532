/*
Name: Zhenjiang Tian
BlazerID: ztian
Lab 04
Compile: gcc -Wall -o lab4 lab4.c
Run: ./lab4 file1 file2
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define BUFFER_SIZE 1024

int main(int argc, char *argv[]) {
    if (argc != 3) {
        fprintf(stderr, "Usage: %s <file1> <file2>\n", argv[0]);
        return 1;
    }

    if (strcmp(argv[1], argv[2]) == 0) {
        fprintf(stderr, "Error: The two filenames must be different.\n");
        return 1;
    }

    FILE *file1 = fopen(argv[1], "a+");
    if (file1 == NULL) {
        perror("Error opening first file");
        return 1;
    }

    FILE *file2 = fopen(argv[2], "r");
    if (file2 == NULL) {
        perror("Error opening second file");
        fclose(file1);
        return 1;
    }

    fseek(file1, -1, SEEK_END);
    int lastChar = fgetc(file1);
    if (lastChar != '\n' && lastChar != EOF) {
        fputc('\n', file1);
    }

    char buffer[BUFFER_SIZE];
    size_t bytesRead;

    while ((bytesRead = fread(buffer, 1, BUFFER_SIZE, file2)) > 0) {
        fwrite(buffer, 1, bytesRead, file1);
    }

    fclose(file1);
    fclose(file2);
    return 0;
}
