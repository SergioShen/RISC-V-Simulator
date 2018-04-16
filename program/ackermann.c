#include "lib.h"

long ackermann(long m, long n) {
    long value = 0;
    if (m == 0)
        value = n + 1;
    else if (n == 0)
        value = ackermann(m - 1, 1);
    else
        value = ackermann(m - 1, ackermann(m, n - 1));
    return value;
}

int main() {
    long m, n;
    print_string("Please give the arguments of ackermann function: ");
    read_long(&m);
    read_long(&n);

    long start_time = time();
    ackermann(m, n);
    long finish_time = time();

    if (finish_time - start_time <= 0) {
        print_string("Insufficient duration - Increase the arguments\n");
        return 1;
    }
    print_string("Arguments: ");
    print_long(m);
    print_char(' ');
    print_long(n);
    print_string(", Duration: ");
    print_long(finish_time - start_time);
    print_string(" sec.\n");
    return 0;
}

