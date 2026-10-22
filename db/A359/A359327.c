/*
 * A359327  Number of divisors of 6*n-5 of form 6*k+5.
 * https://oeis.org/A359327
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   Count the divisors of M = 6·n‑5 that are ≡5 (mod 6)
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    if (n < 1)        /* OEIS offset is 1, but guard anyway */
        return 0;

    /* 6·n‑5 fits in 64 bits for n ≤ 10 000 (and much larger) */
    uint64_t M  = 6ULL * n - 5ULL;
    uint64_t cnt = 0;

    /* iterate over divisors d with d ≤ √M */
    for (uint64_t d = 1; d * d <= M; ++d)
    {
        if (M % d == 0)
        {
            uint64_t d1 = d;
            uint64_t d2 = M / d;

            if (d1 % 6ULL == 5ULL)
                ++cnt;

            if (d2 != d1 && d2 % 6ULL == 5ULL)
                ++cnt;
        }
    }

    return (seq_t)cnt;
}
