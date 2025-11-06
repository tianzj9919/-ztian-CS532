Name: Zhenjiang Tian
BlazerID: ztian
Course: CS 532
Assignment: Lab 10

----------------------------
1. Program Description
----------------------------
This program reads commands from a text file (specified as a command-line argument) 
and executes each command using the fork–exec model. The child process redirects 
its standard output (stdout) and standard error (stderr) to two separate files 
named <pid>.out and <pid>.err, where <pid> is the process ID of the child process.

The parent process records the start and end times of each command execution 
and writes this information, along with the executed command, to a log file 
named output.log. Each record in output.log is written in tab-separated format:
  $  <command> <TAB> <start_time> <TAB> <end_time>

----------------------------
2. Compilation Instructions
----------------------------
To compile the program:
  $ gcc -Wall -Wextra -O2 -std=c11 lab10_solution.c -o lab10

----------------------------
3. Execution Instructions
----------------------------
To run the program:
 $   ./lab10 commands.txt

Where "commands.txt" contains one command per line, for example:
    /bin/echo Hello world
    ls -l
    sleep 2

----------------------------
4. Output Files
----------------------------
For each executed command:
- <pid>.out : Standard output of the child process.
- <pid>.err : Standard error of the child process.
- output.log: A cumulative log file that records each command, start time, and end time.

