#ifndef VECTOR_HEADER
#define VECTOR_HEADER

#include <assert.h>
#include <stdlib.h>
#include <stdio.h>

#define MAX_VEC_CAPACITY 1000

typedef struct Vector {
    int capacity;
    int *items;
    char *name;
} Vector;

void init_vector(Vector *v, char *name, int rows);
void free_vector(Vector *v);

void add_to_vector(Vector *v, int n, int idx);
void print_vector(Vector *v);

#endif