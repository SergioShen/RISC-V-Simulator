//
// Name: main
// Project: RISC_V_Simulator
// Author: Shen Sijie
// Date: 4/6/18
//

#include "utility.h"
#include "machine.h"
#include <cstring>

// Global variables
bool debug_enabled;
Machine *machine;
char *help_message = "Usage: rsv-sim <executable> [options]\nOptions:\n-d debug  : Set debug flag as true\n-h help   : Print this help message and exit\n";


void Initialize(int argc, char **argv) {
    // Global variables initialize
    debug_enabled = false;
    machine = new Machine();

    // Parse cmd arguments
    if (argc < 1) {
        fprintf(stderr, "%s", help_message);
        exit(-1);
    }

    for (int i = 0; i < argc; i++) {
        if (!strcmp(argv[i], "-d") || !strcmp(argv[i], "--debug")) {
            debug_enabled = true;
        } else if (!strcmp(argv[i], "-h") || !strcmp(argv[i], "--help")) {
            printf("%s", help_message);
            exit(0);
        }
    }
    machine->LoadExecutableFile(argv[1]);
}

void CleanSystem() {
    delete machine;
}

int main(int argc, char **argv) {
    Initialize(argc, argv);
    machine->Run();
    CleanSystem();
    return 0;
}
