/* bit_length_u128: the number of bits of n (0 for 0).
 * Shared helper of the OEIS-C database (db/utils). */
#ifndef OEISC_UTILS_BIT_LENGTH_U128_H
#define OEISC_UTILS_BIT_LENGTH_U128_H
#include <stdint.h>

/* number of bits in a non‑zero unsigned __int128 */
static inline unsigned bit_length_u128(unsigned __int128 x) {
    if (x == 0) return 0;
    if (x >> 64) {
        return 64 + (64 - __builtin_clzll((uint64_t)(x >> 64)));
    } else {
        return 64 - __builtin_clzll((uint64_t)x);
    }
}

#endif
