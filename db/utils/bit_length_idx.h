/* bit_length_idx: the number of bits of n (0 for 0).
 * Shared helper of the OEIS-C database (db/utils). */
#ifndef OEISC_UTILS_BIT_LENGTH_IDX_H
#define OEISC_UTILS_BIT_LENGTH_IDX_H
#include <stdint.h>

static inline unsigned bit_length_idx(idx_t x) {
    if (x == 0) return 0;
#if defined(__GNUC__) || defined(__clang__)
    return 64 - __builtin_clzll((unsigned long long)x);
#else
    unsigned bits = 0;
    while (x) {
        x >>= 1;
        ++bits;
    }
    return bits;
#endif
}

#endif
