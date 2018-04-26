#include "../lib.h"

#define malloc mem_alloc
#define rand rand_int
#define srand set_rand_seed

int partition(int *array, int left, int right) {
    int i = left, j = right;
    int pivot_value = array[left];
    while (i != j) {
        while ((array[j] > pivot_value) && (i < j))
            j--;
        if (i < j) {
            array[i] = array[j];
            i++;
        }
        while ((array[i] <= pivot_value) && (i < j))
            i++;
        if (i < j) {
            array[j] = array[i];
            j--;
        }
    }
    array[i] = pivot_value;
    return i;
}

void quick_sort(int *array, int left, int right) {
    if (right <= left)
        return;

    int pivot = partition(array, left, right);
    quick_sort(array, left, pivot - 1);
    quick_sort(array, pivot + 1, right);
}

void random_init(int *array, int length) {
    srand(time());
    for (int i = 0; i < length; i++)
        array[i] = rand();
}

int main() {
    int length;
    print_string("Please give the number of array size: ");
    read_int(&length);

    int *array_point = (int *) malloc(length * sizeof(int));
    random_init(array_point, length);

    long start_time = time();
    quick_sort(array_point, 0, length - 1);
    long finish_time = time();

    if (finish_time - start_time <= 0) {
        print_string("Insufficient duration - Increase the array size\n");
        return 1;
    }
    print_string("Array size: ");
    print_int(length);
    print_string(", Duration: ");
    print_long(finish_time - start_time);
    print_string(" sec.\n");
    return 0;
}
