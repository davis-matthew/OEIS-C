/* bit_length_u64_to_u64: the number of bits of n (0 for 0).
 * Shared helper of the OEIS-C database (db/utils). */
#ifndef OEISC_UTILS_BIT_LENGTH_U64_TO_U64_H
#define OEISC_UTILS_BIT_LENGTH_U64_TO_U64_H
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

static size_t bit_length_u64_to_u64(uint64_t x) {
    size_t b = 0;
    while (x) { b++; x >>= 1; }
    return b;
}

#endif
