/* bit_length_u64: the number of bits of n (0 for 0).
 * Shared helper of the OEIS-C database (db/utils). */
#ifndef OEISC_UTILS_BIT_LENGTH_U64_H
#define OEISC_UTILS_BIT_LENGTH_U64_H
#include <stdbool.h>
#include <stdint.h>

/* Count number of bits in the binary representation of x (i.e. floor(log2(x))+1).
   For x==0 the length is 0. */
static inline int bit_length_u64(uint64_t x) {
    return x == 0 ? 0 : 64 - __builtin_clzll(x);
}

#endif
