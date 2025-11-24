# Makefile for HW4
# Name: Zhenjiang Tian
# BlazerID: ztian

CC      = gcc
CFLAGS  = -Wall -Wextra -O2 -std=c11
LDFLAGS = -lpthread

TARGET  = hw4
SRC     = hw4_ztian.c

.PHONY: all clean run

all: $(TARGET)

$(TARGET): $(SRC)
	$(CC) $(CFLAGS) $(SRC) $(LDFLAGS) -o $(TARGET)

run: $(TARGET)
	./$(TARGET) > result.txt

clean:
	rm -f $(TARGET) result.txt
