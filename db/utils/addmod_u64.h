/* addmod_u64: (a+b) mod m.
 * Shared helper of the OEIS-C database (db/utils). */
#ifndef OEISC_UTILS_ADDMOD_U64_H
#define OEISC_UTILS_ADDMOD_U64_H
#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>

static uint64_t addmod_u64(uint64_t a, uint64_t b, uint64_t p)
{
    uint64_t r = a + b;
    return r >= p ? r - p : r;
}

#endif
