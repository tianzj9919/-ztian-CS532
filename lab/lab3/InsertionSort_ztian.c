#include <stdio.h>
#include <stdlib.h>
#include <string.h>
//gcc InsertionSort_ztian.c -o InsertionSort
//./InsertionSort_ztian

int main() {
    char **arr = NULL; 
    int count = 0; 
    char buffer[1024]; 

    printf("Enter strings line by line (empty to stop):\n");

    while (fgets(buffer, sizeof buffer, stdin)) {
        buffer[strcspn(buffer, "\r\n")] = '\0';
        if (buffer[0] == '\0') break; 

        char **tmp = realloc(arr, (count + 1) * sizeof *arr);
        if (!tmp) {
            fprintf(stderr, "Error: realloc failed.\n");i
            for (int i = 0; i < count; i++) free(arr[i]);
            free(arr);
            return 1;
        }
        arr = tmp;

        arr[count] = malloc(strlen(buffer) + 1);
        strcpy(arr[count], buffer);
        count++;
    }

    for (int i = 1; i < count; i++) {
        char *key = arr[i];
        int j = i - 1;
        while (j >= 0 && strcmp(arr[j], key) > 0) {
            arr[j + 1] = arr[j];
            j--;
        }
        arr[j + 1] = key;
    }

    printf("\nSorted strings:\n");
    for (int i = 0; i < count; i++) {
        printf("%s\n", arr[i]);
        free(arr[i]);
    }
    free(arr);

    return 0;
}
