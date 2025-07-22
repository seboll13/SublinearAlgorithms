CC = gcc
CFLAGS = -O3 -Wall -march=native -ffast-math -funroll-loops
INCLUDES = -I../src
SRC = ../src/vector.c ../src/helpers.c
LIBS = -lm

TARGET = bench_vector
OBJS = bench_vector.c

all: $(TARGET) run

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) $(INCLUDES) -o $(TARGET) $(OBJS) $(SRC) $(LIBS)

run: $(TARGET)
	./$(TARGET)

clean:
	rm -f $(TARGET)

.PHONY: all clean