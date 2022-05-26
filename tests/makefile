CC=gcc
CFLAGS=-c -Wall -Wextra
TESTFLAGS=-lcheck -pthread #-pthread -lm
OBJ=main_test.o vector.o matrix.o tensor.o helpers.o
TARGET=main_test

all: $(TARGET)

$(TARGET): $(OBJ)
	$(CC) $^ -o $@ $(TESTFLAGS)

main_test.o: main_test.c
	$(CC) $(CFLAGS) $^

vector.o: ../C/vector.c
	$(CC) $(CFLAGS) $^

matrix.o: ../C/matrix.c
	$(CC) $(CFLAGS) $^

tensor.o: ../C/tensor.c
	$(CC) $(CFLAGS) $^

helpers.o: ../C/helpers.c
	$(CC) $(CFLAGS) $^

.PHONY: clean

clean:
	rm -f $(OBJ) $(TARGET) && clear

tests: all
	./$(TARGET)