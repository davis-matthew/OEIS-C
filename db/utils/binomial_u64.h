/* binomial_u64: the binomial coefficient C(n, k).
 * Shared helper of the OEIS-C database (db/utils). */
#ifndef OEISC_UTILS_BINOMIAL_U64_H
#define OEISC_UTILS_BINOMIAL_U64_H
#include <stddef.h>
#include <stdint.h>

/* Combination C(n, k) using 128 bit intermediate to avoid overflow.
   Valid for n <= 64 and fits in uint64_t for our use. */
static uint64_t binomial_u64(uint64_t n, uint64_t k)
{
    if (k > n) return 0;
    if (k > n - k) k = n - k;
    __uint128_t res = 1;
    for (uint64_t i = 1; i <= k; ++i) {
        res = res * (n - k + i) / i;
    }
    return (uint64_t)res;
}

#endif
