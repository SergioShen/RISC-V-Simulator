#include "../lib.h"

int main() {
    char string[128];
    char value_char;
    int value_int;

    print_string("Please type a character: ");
    read_char(&value_char);
    print_string("The character you type is ");
    print_char(value_char);
    print_char('\n');

    print_string("Please type a integer: ");
    read_int(&value_int);
    print_string("The integer you type is ");
    print_int(value_int);
    print_char('\n');

    print_string("Please type a string: ");
    read_string(string);
    print_string("The string you type is ");
    print_string(string);
    print_char('\n');

    exit(0);
}
