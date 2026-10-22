/*
 * A348970  a(n) = A003959(n) - A129283(n), where A003959 is multiplicative
 *          with a(p^e) = (p+1)^e and A129283(n) is sum of n and its
 *          arithmetic derivative.
 * https://oeis.org/A348970
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = A003959(n) - (n + A003415(n))
 * Range:     u8 n <= 63, u16 n <= 1535, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   factor_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A003959 is multiplicative with a(p^e) = (p+1)^e;
   A003415(n) = sum over p^e | n of e * (n / p)   */

#include "../utils/factor_u64.h"

seq_t a(idx_t n)
{
    /* treat n as seq_t to avoid accidental truncation */
    seq_t nv = (seq_t)n;

    /* product of (p+1)^e for each prime power in n */
    seq_t prod = 1;
    /* sum of e * (n / p)   */
    seq_t dsum = 0;

    /* if n == 1 the factor list is empty and the loop is skipped */
    uint64_t primes[64];
    uint64_t exps[64];
    size_t   cnt = 0;

    factor_u64((uint64_t)n, primes, exps, &cnt);

    for (size_t i = 0; i < cnt; ++i)
    {
        uint64_t p = primes[i];
        uint64_t e = exps[i];

        /* compute (p+1)^e */
        seq_t part = 1;
        seq_t base = (seq_t)(p + 1);
        for (uint64_t j = 0; j < e; ++j)
            part *= base;

        prod *= part;

        /* accumulate e * (n / p) */
        dsum += (seq_t)e * (nv / (seq_t)p);
    }

    /* A129283(n) = n + A003415(n) = nv + dsum */
    return prod - (nv + dsum);
}
