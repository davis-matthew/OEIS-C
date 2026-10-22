/*
 * A360709  Expansion of Sum_{k>=0} (x^3 / (1 - k*x))^k.
 * https://oeis.org/A360709
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 1 (n = 0); for n > 0 a(n) = Sum_{k=1..⌊n/3⌋} k^(n-3k) *
 *            binomial(n-2k-1, k-1)
 * Range:     u8 n <= 11, u16 n <= 15, u32 n <= 23, u64 n <= 36, u128 n <= 58,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* utilities for binomial coefficient */
#include "../utils/binomial.h"

seq_t a(idx_t n)
{
    if (n == (idx_t)0)          /* a(0) = 1 by definition */
        return (seq_t)1;

    seq_t sum = (seq_t)0;

    /* iterate over k from 1 to floor(n/3) */
    for (idx_t k = (idx_t)1; k <= n / (idx_t)3; ++k)
    {
        idx_t exp   = n - (idx_t)3 * k;               /* n - 3k   */
        seq_t power = (seq_t)1;                       /* compute k^exp */
        for (idx_t i = (idx_t)0; i < exp; ++i)
            power *= (seq_t)k;

        seq_t bin = binomial(n - (idx_t)2 * k - (idx_t)1,
                             k - (idx_t)1);

        sum += power * bin;                           /* accumulation */
    }

    return sum;
}
