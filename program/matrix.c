#include "lib.h"

void randomInit(long **a, long **b, int size) {
    srand(time(0));
    for (int i = 0; i < size; i++) {
        for (int j = 0; j < size; j++) {
            a[i][j] = (long) rand();
            b[i][j] = (long) rand();
        }
    }
}

void multiply(long **a, long **b, long **c, int size) {
    for (int i = 0; i < size; i++) {
        for (int j = 0; j < size; j++) {
            c[i][j] = 0;
            for (int k = 0; k < size; k++) {
                c[i][j] += a[i][k] * b[k][j];
            }
        }
    }
}

int main() {
    int size;
    print_string("Please give the number of matrix size: ");
    read_int(&size);

    long **a = (long **) malloc(size * sizeof(long *));
    long **b = (long **) malloc(size * sizeof(long *));
    long **c = (long **) malloc(size * sizeof(long *));
    for (int i = 0; i < size; i++) {
        a[i] = (long *) malloc(size * sizeof(long));
        b[i] = (long *) malloc(size * sizeof(long));
        c[i] = (long *) malloc(size * sizeof(long));
    }
    randomInit(a, b, size);

    long start_time = time();
    multiply(a, b, c, size);
    long finish_time = time();

    if (finish_time - start_time <= 0) {
        print_string("Insufficient duration - Increase the matrix size\n");
        return 1;
    }
    print_string("Matrix size: ");
    print_int(size);
    print_string(", Duration: ");
    print_long(finish_time - start_time);
    print_string(" sec.\n");
    return 0;
}

