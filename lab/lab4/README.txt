Lab 04 – File Concatenation Program
Author: Zhenjiang Tian
BlazerID: ztian

Compile the program:
$ gcc -Wall -o lab4 lab4.c

Run the program:
$ ./lab4 file1 file2

To create a PDF：
$ enscript lab4.c -o - | ps2pdf - lab4.pdf
