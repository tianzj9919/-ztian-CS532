CS 532 – lab6
Name: <Zhenjiang Tian>
BlazerID: <ztian>

# Sorting `listings.csv` by `host_name` and `price` 

## Build
```bash
gcc -Wall -Wextra -O2 listing.c -o listing
```

## Run
```bash
./listing listings.csv
```
This produces two files in the same directory:
- `sorted_by_host_name.csv`
- `sorted_by_price.csv`

## And using fgets() once to check if the file is empty and also to skip the title line.
