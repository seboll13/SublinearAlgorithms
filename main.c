#include "vector.h"

int main() {
    Vector u, v;

    init_vector(&u, "U", 3);
    init_vector(&v, "V", 3);

    update_vector(&u, 10, 0);
    update_vector(&v, -5, 0);

    print_vector(&u);
    print_vector(&v);

    free_vector(&u);
    free_vector(&v);
}