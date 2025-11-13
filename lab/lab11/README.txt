Name: Zhenjiang Tian
BlazerID: ztian
Course: CS 532 
Lab 11 

1. Description
This program continuously prompts the user to enter a UNIX command and executes the command using the popen() and pclose() functions. 
The program reads the output produced by the command and prints it to the terminal. The loop continues until the user types "quit", at which point the program prints an exit message and terminates.

2. Use the following gcc command:

    gcc -Wall -Wextra -O2 lab11.c -o lab11

This produces the executable file "lab11".

3. Run the program by typing:

    ./lab11

Then enter UNIX commands. Examples:

    Enter command: ls *.c
    Enter command: ls -l pipe[1-3].c
    Enter command: cat file.txt | sort
    Enter command: ls pipe?.c > output.txt

To exit the program:

    Enter command: quit


