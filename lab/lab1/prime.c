/*
Name: Zhenjiang Tian
BlazerID: ztian
Lab 01
Compile: gcc prime.c -o prime
Run: ./prime
*/
#include <stdio.h>
#include <stdbool.h>

int main() {
    int given_number;
    printf("Enter a number: ");
    scanf("%d", &given_number);

    if (given_number <= 1) {
        printf("The number is not prime\n");
        return 0;
    }

    bool isPrime = true;
    for (int i = 2; i * i <= given_number; i++) {
        if (given_number % i == 0) {
            isPrime = false;
            break;
        }
    }

    if (isPrime)
        printf("The number is prime\n");
    else
        printf("The number is not prime\n");

    return 0;
}