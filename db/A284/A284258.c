/*
 * A284258  a(n) = number of distinct prime factors of n that are > the square
 *          of smallest prime factor of n, a(1) = 0.
 * https://oeis.org/A284258
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of distinct prime factors of n that are greater
 *            than spf(n)^2, where spf(n) is the smallest prime factor of n
 * Range:     u8 n <= 10001, u16 n <= 10001, u32 n <= 10001, u64 n <= 10001,
 *            u128 n <= 10001, big any n
 * Validated: standard, pass, 136 terms through n = 8601 (2026-11-01)
 * Helpers:   factor_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/factor_u64.h"

/* Return the smallest prime factor of n   (n > 1) */
static inline idx_t lpf(idx_t n)
{
    if (n % 2 == 0) return 2;
    for (idx_t i = 3; i <= n / i; i += 2)
        if (n % i == 0) return i;
    return n;   /* n itself is prime – should not be called for primes */
}

seq_t a(idx_t n) {
    if (n <= 1) return 0;
    uint64_t nn = (uint64_t)n;
    uint64_t spf = lpf(nn);          /* smallest prime factor */
    uint64_t bound = spf * spf;       /* square of spf */
    uint64_t primes[64], exps[64];
    size_t cnt;
    factor_u64(nn, primes, exps, &cnt); /* distinct prime factors */
    seq_t res = 0;
    for (size_t i = 0; i < cnt; ++i)
        if (primes[i] > bound)
            ++res;
    return res;
}
