/*
 * A369188  Number of squarefree triangular divisors of n.
 * https://oeis.org/A369188
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Sum_{d|n} mu(d)^2 * c(d) where c(d) = 1 if d is a
 *            triangular number, 0 otherwise. Hence a(n) counts the squarefree
 *            divisors of n that are triangular
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 106 terms through n = 9537 (2026-11-01)
 * Helpers:   factor_u64, ipow_i32
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/factor_u64.h"
#include "../utils/ipow_i32.h"

/* Floor square root by binary search */
static inline seq_t floor_sqrt(seq_t x)
{
    seq_t lo = 0, hi = 1;
    while (ipow_i32(hi, 2) <= x)
        hi <<= 1;
    while (lo < hi)
    {
        seq_t mid = (lo + hi) >> 1;
        seq_t mid2 = mid * mid;
        if (mid2 <= x)
            lo = mid + 1;
        else
            hi = mid;
    }
    return lo - 1;
}

static inline uint64_t isqrt_u64(uint64_t n)
{
    uint64_t res   = 0;
    uint64_t bit   = 1ULL << 62;                 /* largest power of two <= sqrt(2^64-1) */
    while (bit > n) bit >>= 2;                  /* find starting bit <= sqrt(n) */
    while (bit)
    {
        if (n >= res + bit)
        {
            n -= res + bit;
            res = (res >> 1) + bit;
        }
        else
        {
            res >>= 1;
        }
        bit >>= 2;
    }
    return res;
}

seq_t a(idx_t n)
{
    if (n < 1) return 0;

    uint64_t primes[64];
    uint64_t exps[64];
    size_t   pf_cnt = 0;

    factor_u64(n, primes, exps, &pf_cnt);

    seq_t count = 0;

    size_t subsets = 1ULL << pf_cnt;  /* safe: pf_cnt <= 20 for n < 2^64 */
    for (size_t mask = 0; mask < subsets; ++mask)
    {
        uint64_t d = 1;
        for (size_t i = 0; i < pf_cnt; ++i)
        {
            if (mask & (1ULL << i))
                d *= primes[i];
        }

        /* test if d is triangular: 8*d + 1 is a perfect square, and its root is odd */
        uint64_t s = 8ULL * d + 1ULL;
        uint64_t t = isqrt_u64(s);
        if (t * t == s && (t & 1ULL))
            ++count;
    }

    return count;
}
