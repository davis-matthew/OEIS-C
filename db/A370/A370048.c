/*
 * A370048  Number of binary strings of length n in which the number of
 *          substrings 00 is one more than that of substrings 01.
 * https://oeis.org/A370048
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Σ_{m=0..⌊(n−1)/3⌋} * [ binom(2m, m+1)·binom(n−1−2m, m) *
 *            + binom(2m+1, m)·binom(n−2−2m, m) ] * Base cases: a(0)=0, a(1)=0
 * Range:     u8 n <= 10, u16 n <= 19, u32 n <= 35, u64 n <= 67, u128 n <= 132,
 *            big any n
 * Helpers:   binom
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binom.h"

seq_t a(idx_t n)
{
    if (n < 2) return (seq_t)0;
    idx_t m_max = (n - 1) / 3;      /* floor((n-1)/3) */
    seq_t sum  = 0;
    for (idx_t m = 0; m <= m_max; ++m)
    {
        seq_t t1 = binom(2 * m, m + 1);
        t1 *= binom(n - 1 - 2 * m, m);

        seq_t t2 = binom(2 * m + 1, m);
        t2 *= binom(n - 2 - 2 * m, m);

        sum += t1 + t2;
    }
    return sum;
}
