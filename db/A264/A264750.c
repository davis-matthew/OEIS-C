/*
 * A264750  Number of sequences of 5 throws of an n-sided die (with faces
 *          numbered 1, 2, ..., n) in which the sum of the throws first
 *          reaches or exceeds n on the 5th throw.
 * https://oeis.org/A264750
 *
 * Offset:    5    (a(5) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (n - 4)*(n - 3)*(n - 2)*(n - 1)*(4*n + 5)/120 (n ≥ 5)
 * Range:     u8 n <= 6, u16 n <= 18, u32 n <= 168, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    if (n < 5) return (seq_t)0;

    seq_t tn = (seq_t)n;

    /* Compute (n-4)(n-3)(n-2)(n-1)/24 */
    seq_t c = tn - 4;
    c *= tn - 3;
    c *= tn - 2;
    c *= tn - 1;
    c /= (seq_t)24;

    seq_t m = (seq_t)(4 * tn + 5);

    seq_t c_div = c / (seq_t)5;
    seq_t rem   = c % (seq_t)5;

    seq_t result = c_div * m;
    result += (rem * m) / (seq_t)5;

    return result;
}
