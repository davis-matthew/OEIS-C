/*
 * A387573  Total number of runs of 1's in all length n binary words in which
 *          1's occur in runs of at least 3.
 * https://oeis.org/A387573
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Sum_{i=1..floor((n+1)/4)} i*binomial(n+1-2*i, 2*i)
 * Range:     u8 n <= 10, u16 n <= 21, u32 n <= 43, u64 n <= 88, u128 n <= 178,
 *            big any n
 * Helpers:   binom
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binom.h"

seq_t a(idx_t n)
{
    seq_t sum = 0;
    idx_t max_i = (n + 1) / 4;
    for (idx_t i = 1; i <= max_i; ++i)
    {
        /* binom is provided by utils/binom.h */
        seq_t val = binom(n + 1 - 2 * i, 2 * i);
        sum += (seq_t)i * val;
    }
    return sum;
}
