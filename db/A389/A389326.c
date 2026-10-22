/*
 * A389326  a(n) = Sum_{k=0..floor(n/2)} binomial(n,k) *
 *          binomial(n+2*k-1,n-2*k).
 * https://oeis.org/A389326
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=0..⌊n/2⌋} binomial(n,k) * binomial(n+2k-1, n-2k)
 * Range:     u8 n <= 5, u16 n <= 9, u32 n <= 16, u64 n <= 31, u128 n <= 60,
 *            big any n
 * Helpers:   binom
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binom.h"

seq_t a(idx_t n)
{
    /* Handle the special case n = 0 */
    if (n == 0)
        return (seq_t)1;

    seq_t sum = 0;
    idx_t half = n >> 1;            /* floor(n/2) */

    for (idx_t k = 0; k <= half; ++k)
    {
        seq_t t1 = binom(n, k);
        seq_t t2 = binom(n + 2 * k - 1, n - 2 * k);
        sum += t1 * t2;
    }

    return sum;
}
