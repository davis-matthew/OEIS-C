/*
 * A358296  Row 3 of the array in A115009.
 * https://oeis.org/A358296
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 5*n*(n+1)/2 - 3 + floor(n/2)
 * Range:     u8 n <= 9, u16 n <= 60, u32 n <= 60, u64 n <= 60, u128 n <= 60,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* Triangular number t = n*(n+1)/2 */
    seq_t t = (seq_t)n * ((seq_t)n + 1) / 2;

    /* a(n) = 5*t + floor(n/2) - 3 */
    return 5 * t + (seq_t)(n >> 1) - (seq_t)3;
}
