/* bigomega_u64: the number of prime factors of n, with multiplicity.
 * Shared helper of the OEIS-C database (db/utils). */
#ifndef OEISC_UTILS_BIGOMEGA_U64_H
#define OEISC_UTILS_BIGOMEGA_U64_H
#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>

/* trial‑division prime‑factor count for a 64‑bit integer */
static seq_t bigomega_u64(uint64_t m)
{
    if (m <= 1) return (seq_t)0;
    seq_t count = 0;

    while ((m & 1ULL) == 0) {
        ++count;
        m >>= 1;
    }
    for (uint64_t d = 3; d * d <= m; d += 2) {
        while (m % d == 0) {
            ++count;
            m /= d;
        }
    }
    if (m > 1) ++count;   /* last prime factor */
    return count;
}

#endif
