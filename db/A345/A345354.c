/*
 * A345354  a(n) = Sum_{p|n, p prime} omega(n/p).
 * https://oeis.org/A345354
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{p|n, p prime} ω(n/p) where ω(m) is the number of
 *            distinct prime factors of m
 * Range:     u8 n <= 92, u16 n <= 92, u32 n <= 92, u64 n <= 92, u128 n <= 92,
 *            big any n
 * Helpers:   count_distinct_primes, factor_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/factor_u64.h"
#include "../utils/count_distinct_primes.h"

/* Compute a(n) for n >= 1. */
seq_t a(idx_t n)
{
    if (n <= 1) return 0;           /* a(1) = 0 and n < 1 is irrelevant */

    /* Factor n into its distinct prime factors. */
    /* 64 distinct primes is an ample upper bound for n within the
       target range (≤ ~10^12).  For larger n the function still
       works as long as the actual number of distinct primes is <64. */
    uint64_t primes[64];
    uint64_t exps[64];
    size_t cnt = 0;
    factor_u64(n, primes, exps, &cnt);

    seq_t ans = 0;
    for (size_t i = 0; i < cnt; ++i)
    {
        uint64_t p = primes[i];
        uint64_t div = n / p;
        /* ω(div) is the number of distinct prime factors of div. */
        ans += (seq_t)count_distinct_primes(div);
    }
    return ans;
}
