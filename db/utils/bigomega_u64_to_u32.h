/* bigomega_u64_to_u32: the number of prime factors of n, with multiplicity.
 * Shared helper of the OEIS-C database (db/utils). */
#ifndef OEISC_UTILS_BIGOMEGA_U64_TO_U32_H
#define OEISC_UTILS_BIGOMEGA_U64_TO_U32_H
#include <stdint.h>

/* Count prime factors (with multiplicity) of a 64‑bit integer x.
   This routine uses trial division with the 6k±1 optimization. */
static unsigned int bigomega_u64_to_u32(uint64_t x)
{
    unsigned int count = 0;

    /* factor 2 */
    while ((x & 1ULL) == 0) {
        ++count;
        x >>= 1;
    }
    /* factor 3 */
    while (x % 3ULL == 0) {
        ++count;
        x /= 3;
    }

    /* factor 6k±1 */
    for (uint64_t f = 5; f * f <= x; f += 6) {
        while (x % f == 0) {
            ++count;
            x /= f;
        }
        uint64_t fp = f + 2;
        while (x % fp == 0) {
            ++count;
            x /= fp;
        }
    }
    if (x > 1) ++count;          /* remaining prime factor */
    return count;
}

#endif
