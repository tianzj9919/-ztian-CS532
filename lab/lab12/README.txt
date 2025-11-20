Name: Zhenjiang Tian  
BlazerID: ztian  
Course: CS 532 
Lab 12 

Description of Modifications
The original program used global variables (a, sum, N, size, tid) and passed only a thread ID to pthread_create(). The homework requires removing the use of these global variables and passing a structure to each thread instead.

How to Compile

Use the following command:

    gcc -O -Wall pthread_sum_struct.c -lpthread -o pthread_sum_struct


How to Run

Run the program with the number of elements and the number of threads:

    ./pthread_sum_struct <#elements> <#threads>

Example:

    ./pthread_sum_struct 1000 4

