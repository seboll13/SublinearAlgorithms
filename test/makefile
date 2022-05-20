CC=gcc
CFLAGS=-c -Wall -Wextra
TESTFLAGS=-lcheck -pthread #-pthread -lm
OBJ=vector.o vector_test.o
TARGET=vector_test

all: $(TARGET)

$(TARGET): $(OBJ)
	$(CC) $^ -o $@ $(TESTFLAGS)

vector.o: ../C/vector.c
	$(CC) $(CFLAGS) $^

vector_test.o : vector_test.c
	$(CC) $(CFLAGS) $^ $(TESTFLAGS)

.PHONY: clean

clean:
	rm -f $(OBJ) $(TARGET)