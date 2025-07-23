CC=gcc
CFLAGS=-Wall -Wextra -O2
INCLUDES=-I../src
SRC=../src/vector.c ../src/matrix.c ../src/helpers.c
LIBS=-lm

TARGETS=bench_vector bench_matrix run_vector run_matrix
OBJS=bench_vector.c

all: $(TARGETS)

bench_vector: bench_vector.c $(SRC)
	$(CC) $(CFLAGS) $(INCLUDES) -o $@ $< $(SRC) $(LIBS)

bench_matrix: bench_matrix.c $(SRC)
	$(CC) $(CFLAGS) $(INCLUDES) -o $@ $< $(SRC) $(LIBS)

run_vector: bench_vector
	./bench_vector

run_matrix: bench_matrix
	./bench_matrix

clean:
	rm -f $(TARGETS)

.PHONY: all clean