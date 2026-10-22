/*
 * A364097  Expansion of Sum_{k>0} k * x^(3*k-2) / (1 - x^(5*k-4)).
 * https://oeis.org/A364097
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (1/5) * Sum_{d | 5*n-2, d == 1 (mod 5)} (d+4)
 * Range:     u8 n <= 93, u16 n <= 93, u32 n <= 93, u64 n <= 93, u128 n <= 93,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    if (n == 0)     /* offset is 1, undefined below 1 */
        return 0;

    /* Compute m = 5*n-2 using 128-bit arithmetic to avoid overflow on
       intermediate multiplication.  The result fits into a 64‑bit unsigned
       integer for the input sizes used in the tests. */
    uint64_t m = (uint64_t)(((__uint128_t)5 * n) - 2);

    seq_t sum = 0;
    for (uint64_t d = 1; (__uint128_t)d * d <= m; ++d)
    {
        if (m % d != 0)
            continue;

        uint64_t d1 = d;
        uint64_t d2 = m / d;

        if (d1 % 5 == 1)
            sum += (seq_t)d1 + (seq_t)4;

        if (d2 != d1 && d2 % 5 == 1)
            sum += (seq_t)d2 + (seq_t)4;
    }

    return sum / (seq_t)5;
}
