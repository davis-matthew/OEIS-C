/* bit_length: the number of bits of n (0 for 0).
 * Shared helper of the OEIS-C database (db/utils). */
#ifndef OEISC_UTILS_BIT_LENGTH_H
#define OEISC_UTILS_BIT_LENGTH_H
#include <stdint.h>

static inline unsigned bit_length(seq_t x) {
    if (x == 0) return 0;
    unsigned bits = 0;
    while (x) {
        x >>= 1;
        ++bits;
    }
    return bits;
}

#endif
