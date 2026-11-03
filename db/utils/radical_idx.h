/* radical_idx: the product of the distinct primes dividing n.
 * Shared helper of the OEIS-C database (db/utils). */
#ifndef OEISC_UTILS_RADICAL_IDX_H
#define OEISC_UTILS_RADICAL_IDX_H
#include <stdint.h>
#include <stdlib.h>

/* Compute the square‑free kernel of n.  Since n ≤ 10000 in the test set, 
   a simple trial division is fast enough. */
static idx_t radical_idx(idx_t n)
{
    idx_t rad = 1;
    idx_t m = n;
    for (idx_t p = 2; p * p <= m; ++p) {
        if (m % p == 0) {
            rad *= p;
            while (m % p == 0)
                m /= p;
        }
    }
    if (m > 1)          /* remaining prime factor */
        rad *= m;
    return rad;
}

#endif
