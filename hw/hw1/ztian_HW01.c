#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdbool.h>
#include <math.h>
//gcc ztian_HW01.c -o hw01 -lm
//./hw01
int  sumOfDigits(int n);
int  UABMaxMinDiff(const int *arr, int size);
int* replaceEvenWithZero(const int *arr, int size);
bool perfectSquare(int n);
int  countVowels(const char *s);

int sumOfDigits(int n) {
    if (n <= 0) return -1;
    int sum = 0;
    while (n > 0) {
        sum += n % 10;
        n /= 10;
    }
    return sum;
}

int UABMaxMinDiff(const int *arr, int size) {
    if (arr == NULL || size <= 0) return -1;
    int min = arr[0], max = arr[0];
    for (int i = 1; i < size; i++) {
        if (arr[i] < min) min = arr[i];
        if (arr[i] > max) max = arr[i];
    }
    return max - min;
}

int* replaceEvenWithZero(const int *arr, int size) {
    if (arr == NULL || size <= 0) return NULL;
    int *out = (int*)malloc(sizeof(int) * size);
    if (!out) return NULL;
    for (int i = 0; i < size; i++) {
        out[i] = (arr[i] % 2 == 0) ? 0 : arr[i];
    }
    return out;
}

bool perfectSquare(int n) {
    if (n < 0) return false;
    int k = (int)(sqrt((double)n) + 0.5);
    return (k * k == n);
}

int countVowels(const char *s) {
    if (s == NULL) return 0;
    int count = 0;
    for (const char *p = s; *p; ++p) {
        char c = (char)tolower((unsigned char)*p);
        if (c=='a' || c=='e' || c=='i' || c=='o' || c=='u') {
            count++;
        }
    }
    return count;
}

int main(void){
    //Q1
    int n1 = 123;
    int n2 = 405;
    int n3 = 0;
    int n4 = 7;
    int n5 = -308;
    printf("sumOfDigits(%d) = %d\n", n1, sumOfDigits(n1));
    printf("sumOfDigits(%d) = %d\n", n2, sumOfDigits(n2));
    printf("sumOfDigits(%d) = %d\n", n3, sumOfDigits(n3));
    printf("sumOfDigits(%d) = %d\n", n4, sumOfDigits(n4));
    printf("sumOfDigits(%d) = %d\n", n5, sumOfDigits(n5));
    //Q2
    int arr1[] = {3,7,2,9};
    int diff1 = UABMaxMinDiff(arr1, (int)(sizeof(arr1)/sizeof(arr1[0])));
    printf("UABMaxMinDiff([3,7,2,9]) = %d\n", diff1);
    int arr2[] = {5,5,5,5,5,5};
    int diff2 = UABMaxMinDiff(arr2, (int)(sizeof(arr2)/sizeof(arr2[0])));
    printf("UABMaxMinDiff([5,5,5,5,5,5]) = %d\n", diff2);
    int arr3[] = {-2,4,-1,6,5};
    int diff3 = UABMaxMinDiff(arr3, (int)(sizeof(arr3)/sizeof(arr3[0])));
    printf("UABMaxMinDiff([-2,4,-1,6,5]) = %d\n", diff3);
    //Q3
    int arr4[] = {1,2,3,4};
    int size4 = (int)(sizeof(arr4)/sizeof(arr4[0]));
    int *repl = replaceEvenWithZero(arr4, size4);
    printf("replaceEvenWithZero([1,2,3,4]) = [");
    if (repl) {
        for (int i = 0; i < size4; i++) {
            printf("%s%d", (i ? ", " : ""), repl[i]);
        }
    }
    printf("]\n");
    free(repl); repl = NULL;

    int arr5[] = {2,4,6};
    int size5 = (int)(sizeof(arr5)/sizeof(arr5[0]));
    repl = replaceEvenWithZero(arr5, size5);
    printf("replaceEvenWithZero([2,4,6]) = [");
    if (repl) {
        for (int i = 0; i < size5; i++) {
            printf("%s%d", (i ? ", " : ""), repl[i]);
        }
    }
    printf("]\n");
    free(repl); repl = NULL;

    int arr6[] = {1,3,5};
    int size6 = (int)(sizeof(arr6)/sizeof(arr6[0]));
    repl = replaceEvenWithZero(arr6, size6);
    printf("replaceEvenWithZero([1,3,5]) = [");
    if (repl) {
        for (int i = 0; i < size6; i++) {
            printf("%s%d", (i ? ", " : ""), repl[i]);
        }
    }
    printf("]\n");
    free(repl); repl = NULL;

    //Q4
    int p1 = 16, p2 = 15, p3 = 25,p4 = 36;
    printf("perfectSquare(%d) = %s\n", p1, perfectSquare(p1) ? "True" : "False");
    printf("perfectSquare(%d) = %s\n", p2, perfectSquare(p2) ? "True" : "False");
    printf("perfectSquare(%d) = %s\n", p3, perfectSquare(p3) ? "True" : "False");
    printf("perfectSquare(%d) = %s\n", p4, perfectSquare(p4) ? "True" : "False");
    //Q5
    const char *s1 = "Hello World";
    const char *s2 = "UAB CS";
    const char *s3 = "Python";
    const char *s4 = "aeiou";
    printf("countVowels(\"%s\") = %d\n", s1, countVowels(s1));
    printf("countVowels(\"%s\") = %d\n", s2, countVowels(s2));
    printf("countVowels(\"%s\") = %d\n", s3, countVowels(s3));
    printf("countVowels(\"%s\") = %d\n", s4, countVowels(s4));


    return 0;
}