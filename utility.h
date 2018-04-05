//
// Name: utility
// Project: RISC_V_Simulator
// Author: Shen Sijie
// Date: 4/6/18
//

#ifndef RISC_V_SIMULATOR_UTILITY_H
#define RISC_V_SIMULATOR_UTILITY_H

#include <cstdio>
#include <cstdlib>
#include <cstdarg>

typedef long int64;
typedef int int32;
typedef short int16;
typedef char int8;


#define ASSERT(condition)                                                                         \
    if (!(condition)) {                                                                           \
        fprintf(stderr, "Assertion failed: line %d, file \"%s\"\n", __LINE__, __FILE__);          \
        fflush(stderr);                                                                           \
        abort();                                                                                  \
    }                                                                                             \

extern bool debug_enabled;

#define DEBUG(...)                                                                                \
    if (debug_enabled) {                                                                          \
        printf(__VA_ARGS__);                                                                      \
    }                                                                                             \

#endif //RISC_V_SIMULATOR_UTILITY_H
