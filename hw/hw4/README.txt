Name: Zhenjiang Tian
BlazerID: ztian
Course: CS 532 
Assignment: HW4

1. Program Description
This program implements a multi-threaded producer/consumer system using:
- Processes: fork()
- Threads: POSIX pthreads
- Inter-process communication: pipe()
- Synchronization: mutexes (and semaphores conceptually)
- Signals: SIGUSR1 to coordinate between parent and child

Workflow:
1) The parent process creates a pipe and then forks a child process.
2) The parent process:
   - Creates 10 producer threads.
   - Each producer thread generates 500 unique random integers in the range [0, 1000].
   - Each thread writes its integers to the write-end of the pipe.
   - A mutex is used to protect write() so that multiple producers do not corrupt the pipe data.
   - Each producer prints progress information and a completion message.
3) After all producer threads finish, the parent:
   - Closes the write-end of the pipe.
   - Sends a SIGUSR1 signal to the child process to notify that all numbers have been generated.
4) The child process:
   - Installs a SIGUSR1 signal handler.
   - Waits (using pause()) until it receives the signal from the parent.
   - After receiving SIGUSR1, it creates 20 consumer threads.
   - Each consumer thread reads 250 integers from the read-end of the pipe and computes their sum.
   - Each consumer prints its completion message and its partial sum.
5) After all 20 consumer threads complete, the child process:
   - Sums up the 20 partial sums.
   - Computes the average (total / 20).
   - Prints the final average to standard output (this is redirected to a text file).
This design satisfies the requirements for:
- Process management
- Pipe communication
- Multi-threaded producers and consumers
- Synchronization to avoid race conditions
- Signal-based coordination between parent and child
- Average computation of the consumer results

2. To compile the program, simply run:

   $ make

This will produce an executable named:

    hw4


3. Run the program and redirect the final output to a file:

    make run

This command is equivalent to:

    ./hw4 > result.txt


4. Files Included
- hw4_ztian.c 
- Makefile
- README.txt 
- report.txt 


