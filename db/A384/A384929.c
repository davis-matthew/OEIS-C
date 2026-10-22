/*
 * A384929  a(n) = (1/2) * Sum_{k=1..n} (n+k)! / k!.
 * https://oeis.org/A384929
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = ((2*n+1)!/(n+1)! - n!)/2 (offset 1)
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 8, u64 n <= 14, u128 n <= 24,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;          /* outside the defined offset */

    /* compute n! */
    seq_t fact = 1;
    for (idx_t i = 2; i <= n; ++i)
        fact *= (seq_t)i;

    /* compute binomial coefficient C(2n+1, n+1) */
    seq_t bin = 1;
    for (idx_t i = 1; i <= n + 1; ++i)
        bin = bin * (seq_t)(n + i) / (seq_t)i;

    /* a(n) = n! * (bin - 1) / 2 */
    seq_t term = (bin - (seq_t)1) * fact;
    return term / (seq_t)2;
}
