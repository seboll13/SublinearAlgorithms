CC=gcc
CFLAGS=-c -Wall -Wextra
OBJ=main.o vector.o
TARGET=main

all: $(TARGET)

$(TARGET): $(OBJ)
	$(CC) $^ -o $(TARGET)

vector.o: vector.c
	$(CC) $(CFLAGS) $^

main.o: main.c
	$(CC) $(CFLAGS) $^

.PHONY: clean

clean:
	rm -f $(OBJ) $(TARGET)