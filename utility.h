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

typedef signed char int8_t;
typedef short int int16_t;
typedef int int32_t;
typedef long int int64_t;

typedef unsigned char uint8_t;
typedef unsigned short int uint16_t;
typedef unsigned int uint32_t;
typedef unsigned long int uint64_t;


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
        fflush(stdout);                                                                           \
    }                                                                                             \


#define FATAL(...)                                                                                \
    {                                                                                             \
        fprintf(stderr, __VA_ARGS__);                                                             \
        fflush(stderr);                                                                           \
        abort();                                                                                  \
    }                                                                                             \

#endif //RISC_V_SIMULATOR_UTILITY_H
