/*
 * A390566  Upper (1/3,1/2) midsequence of (n^2) and (n^3); see Comments.
 * https://oeis.org/A390566
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = ceil(n^2/3 + n^3/2)
 * Range:     u8 n <= 4, u16 n <= 27, u32 n <= 43, u64 n <= 43, u128 n <= 43,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* Compute a(n) = ceil(n^2/3 + n^3/2) for any n ≥ 0.
   Uses only integer arithmetic. */
seq_t a(idx_t n)
{
    /* Convert n to seq_t once, to avoid repeated conversions. */
    seq_t N = (seq_t)n;
    /* numerator = n^2 * (2 + 3*n)   (common denominator 6) */
    seq_t num = N * N * ((seq_t)2 + (seq_t)3 * N);
    /* ceil division by 6: (num + 5) / 6 */
    return (num + (seq_t)5) / (seq_t)6;
}
