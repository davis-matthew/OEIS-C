/*
 * A243980  Four times the sum of all divisors of all positive integers <= n.
 * https://oeis.org/A243980
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 4 * Σ_{k=1..n} σ(k) = 4 * Σ_{d=1..n} d * ⌊n/d⌋
 * Range:     u8 n <= 8, u16 n <= 140, u32 n <= 7342, u64 n <= 7342,
 *            u128 n <= 7342, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* a(0) is undefined, return 0 safely */

    seq_t sum = 0;
    for (idx_t d = 1; d <= n; ++d)
    {
        seq_t q = (seq_t)n / (seq_t)d;  /* floor(n/d) */
        seq_t term = (seq_t)d * q;
        sum += term;
    }
    return sum * 4;
}
