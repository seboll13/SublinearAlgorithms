CC=gcc
CFLAGS=-c -Wall -Wextra -O3#-mcpu=apple-m1 -mtune=apple-m1 -funroll-loops
OBJ=main.o vector.o projections.o matrix.o tensor.o helpers.o
TARGET=main

all: $(TARGET)

run: $(TARGET)
	./$(TARGET)

$(TARGET): $(OBJ)
	$(CC) $^ -o $@

vector.o: vector.c
	$(CC) $(CFLAGS) $^

projections.o: projections.c
	$(CC) $(CFLAGS) $^

matrix.o: matrix.c
	$(CC) $(CFLAGS) $^

tensor.o: tensor.c
	$(CC) $(CFLAGS) $^

helpers.o: helpers.c
	$(CC) $(CFLAGS) $^

main.o: main.c
	$(CC) $(CFLAGS) $^

library: $(OBJ)
	$(CC) -fPIC -shared $^ -o lib$(TARGET).so

.PHONY: all clean

clean:
	rm -f $(OBJ) $(TARGET)
	clear
