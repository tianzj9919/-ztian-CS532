Name: Zhenjiang Tian
BlazerID: ztian
Course: CS 532
Lab9

Description:
This program demonstrates the use of the fork-exec-wait pattern in C. 
The parent process ignores SIGINT and SIGTSTP and only terminates upon receiving SIGQUIT (Ctrl-\).
The child process executes the given command and responds to Ctrl-C and Ctrl-Z normally.

To Compile:
    gcc -Wall forkexecvp.c -o forkexecvp

To Run:
    ./forkexecvp <command> [args]

Example:
    ./forkexecvp sleep 100

    Then:
    - Press Ctrl-C: child process is interrupted, parent keeps waiting.
    - Press Ctrl-Z: child process is suspended, parent keeps waiting.
    - Press Ctrl-\\: parent sends SIGQUIT to child and exits cleanly.
