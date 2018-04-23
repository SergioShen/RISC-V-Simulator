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
bool initializing;
bool debug_enabled;
bool interactive;
Machine *machine;

void PrintHelpMessage(FILE *file) {
    fprintf(file, "Usage: rsv-sim [options] <executable>\n");
    fprintf(file, "Options:\n");
    fprintf(file, "-d debug           : Set debug flag as true\n");
    fprintf(file, "-h help            : Print this help message and exit\n");
    fprintf(file, "-i interactive     : Interactive debug mode\n");
}

void PrintInteractiveHelpMessage(FILE *file) {
    fprintf(file, "Interactive debug mode usage:\n");
    fprintf(file, "reg                 : Display all registers\n");
    fprintf(file, "m(mem) <hex addr>   : Show memory content at <hex addr>\n");
    fprintf(file, "s(single)           : Run single steps\n");
    fprintf(file, "r(run) <count>      : Run <count> steps\n");
    fprintf(file, "q(quit)             : Quit\n");
    fprintf(file, "h(help)             : Show this help message\n");
}


void Initialize(int argc, char **argv) {
    // Global variables initialize
    debug_enabled = false;
    interactive = false;
    initializing = true;
    machine = new Machine();

    // Parse cmd arguments
    if (argc < 1) {
        PrintHelpMessage(stderr);
        exit(-1);
    }

    for (int i = 1; i < argc; i++) {
        if (!strcmp(argv[i], "-d") || !strcmp(argv[i], "--debug")) {
            debug_enabled = true;
        } else if (!strcmp(argv[i], "-h") || !strcmp(argv[i], "--help")) {
            PrintHelpMessage(stdout);
            exit(0);
        } else if (!strcmp(argv[i], "-i") || !strcmp(argv[i], "--interactive")) {
            interactive = true;
            debug_enabled = true;
        } else {
            // It should be the executable file name
            ASSERT(i == argc - 1);
            machine->LoadExecutableFile(argv[i]);
        }
    }
    initializing = false;
}

void Run() {
    for (;;) {
        machine->OneCycle();
        if (machine->exit_flag)
            break;
    }
}

void InteractiveRun() {
    char cmd[8];
    int64_t cmd_arg, value;

    PrintInteractiveHelpMessage(stdout);
    for (;;) {
        scanf("%s", cmd);
        if (!strcmp(cmd, "reg")) {
            machine->PrintRegisters();
        } else if (!strcmp(cmd, "m") || !strcmp(cmd, "mem")) {
            scanf("%lx", &cmd_arg);
            machine->main_memory->ReadMemory(cmd_arg, 8, &value);
            printf("Value at memory %lx: %16.16lx\n", cmd_arg, value);
        } else if (!strcmp(cmd, "s") || !strcmp(cmd, "single")) {
            machine->OneCycle();
            if (machine->exit_flag)
                break;
        } else if (!strcmp(cmd, "r") || !strcmp(cmd, "run")) {
            scanf("%ld", &cmd_arg);
            ASSERT(cmd_arg > 0);
            for (int i = 0; i < cmd_arg; i++) {
                machine->OneCycle();
                if (machine->exit_flag)
                    break;
            }
        } else if (!strcmp(cmd, "q") || !strcmp(cmd, "quit")) {
            break;
        } else if (!strcmp(cmd, "h") || !strcmp(cmd, "help")) {
            PrintInteractiveHelpMessage(stdout);
        }
    }
}

void CleanSystem() {
    delete machine;
}

int main(int argc, char **argv) {
    Initialize(argc, argv);
    if (interactive)
        InteractiveRun();
    else
        Run();
    CleanSystem();
    return 0;
}
