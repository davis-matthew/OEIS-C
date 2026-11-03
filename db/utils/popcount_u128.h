/* popcount_u128: the number of 1 bits of n.
 * Shared helper of the OEIS-C database (db/utils). */
#ifndef OEISC_UTILS_POPCOUNT_U128_H
#define OEISC_UTILS_POPCOUNT_U128_H
#include <stdbool.h>
#include <stdint.h>

/* Count ones in a 128‑bit unsigned integer */
static inline unsigned popcount_u128(unsigned __int128 x)
{
    unsigned long long lo = (unsigned long long)x;
    unsigned long long hi = (unsigned long long)(x >> 64);
    return __builtin_popcountll(lo) + __builtin_popcountll(hi);
}

#endif
