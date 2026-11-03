/* smallest_prime_factor_idx: the smallest prime factor of n.
 * Shared helper of the OEIS-C database (db/utils). */
#ifndef OEISC_UTILS_SMALLEST_PRIME_FACTOR_IDX_H
#define OEISC_UTILS_SMALLEST_PRIME_FACTOR_IDX_H
#include <stdbool.h>
#include <stdint.h>

static idx_t smallest_prime_factor_idx(idx_t x)
{
    if (x < 2) return x;
    if ((x & 1) == 0) return 2;
    for (idx_t d = 3; d * d <= x; d += 2)
        if (x % d == 0) return d;
    return x;
}

#endif
