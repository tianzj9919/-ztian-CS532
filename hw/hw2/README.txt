CS 532 – HW2: search
Name: <Zhenjiang Tian>
BlazerID: <ztian>

Files Included
- search.c
- Makefile
- README.txt

Build Instructions
Terminal:
  make
  #gcc -Wall -Wextra -std=c11 -O2 search.c -o search

Program Usage
  ./search [options] [start_dir]

Options:
  -S
  -s <bytes>
  -f <pattern> <depth>
  -t <f|d>

Exit Codes
----------
- 0: success
- 1: argument error or overall failure

