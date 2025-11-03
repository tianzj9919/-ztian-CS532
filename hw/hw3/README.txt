Name: Zhenjiang Tian
BlazerID: ztian
Project #: HW3 CS 532

To compile:
$ make

To run):
$ ./hw3 /path/to/directory
$ ./hw3 -u username /path/to/directory

Program Description:
- The program validates the directory argument, traverses entries (non-recursive), and for each regular file spawns one child process via fork().
- Each child prints: file name and size; for ".txt" files it also prints the word count.
- The parent process waits for all children and then exits.
- With "-u <username>", only files owned by that user are shown, and the output includes "Owner: <username>".

Edge Cases & Error Handling:
- Missing/invalid directory -> prints an error message and exits.
- Non-regular entries are skipped.
- Word counting gracefully handles empty files; unreadable files print a warning and show "Words: N/A".
- Fork failures cause the program to print an error and terminate after waiting for already-spawned children.

