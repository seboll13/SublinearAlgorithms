CC=gcc
CFLAGS=-c -Wall -Wextra
#TESTFLAGS=-lcheck -pthread -pthread -lm
OBJ=main.o vector.o projections.o matrix.o
TARGET=main

all: $(TARGET)

$(TARGET): $(OBJ)
	$(CC) $^ -o $@

vector.o: vector.c
	$(CC) $(CFLAGS) $^

projections.o: projections.c
	$(CC) $(CFLAGS) $^

matrix.o: matrix.c
	$(CC) $(CFLAGS) $^

main.o: main.c
	$(CC) $(CFLAGS) $^

.PHONY: clean

clean:
	rm -f $(OBJ) $(TARGET)