CC=gcc
PKG_CFLAGS=$(shell pkg-config --cflags check)
PKG_LIBS =$(shell pkg-config --libs check)
CFLAGS=-Wall -Wextra $(PKG_CFLAGS)
LDFLAGS=-pthread $(PKG_LIBS)
OBJ=main_test.o vector.o matrix.o tensor.o helpers.o
TARGET=main_test

all: $(TARGET)

$(TARGET): $(OBJ)
	$(CC) $^ -o $@ $(LDFLAGS)

main_test.o: main_test.c
	$(CC) $(CFLAGS) -c $<

vector.o: ../src/vector.c
	$(CC) $(CFLAGS) -c $^

matrix.o: ../src/matrix.c
	$(CC) $(CFLAGS) -c $^

tensor.o: ../src/tensor.c
	$(CC) $(CFLAGS) -c $^

helpers.o: ../src/helpers.c
	$(CC) $(CFLAGS) -c $^

.PHONY: clean

clean:
	rm -f $(OBJ) $(TARGET) && clear

tests: all
	./$(TARGET)