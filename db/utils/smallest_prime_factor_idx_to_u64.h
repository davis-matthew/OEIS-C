/* smallest_prime_factor_idx_to_u64: the smallest prime factor of n.
 * Shared helper of the OEIS-C database (db/utils). */
#ifndef OEISC_UTILS_SMALLEST_PRIME_FACTOR_IDX_TO_U64_H
#define OEISC_UTILS_SMALLEST_PRIME_FACTOR_IDX_TO_U64_H
#include <stdint.h>

/* Least prime factor of n (for n <= 2^63-1).  Returns 1 for n=1. */
static uint64_t smallest_prime_factor_idx_to_u64(idx_t n)
{
    if (n < 2)
        return n;
    if ((n & 1) == 0)
        return 2;
    for (idx_t d = 3; (__uint128_t)d * d <= n; d += 2)
        if (n % d == 0)
            return d;
    return n;                /* n is prime */
}

#endif
