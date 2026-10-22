/*
 * A394869  a(n) = floor(n*(sqrt(n - 1) + 2)/2).
 * https://oeis.org/A394869
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = floor(n*(sqrt(n-1)+2)/2)
 * Range:     u8 n <= 4, u16 n <= 25, u32 n <= 60, u64 n <= 60, u128 n <= 60,
 *            big any n
 * Helpers:   floor_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/floor_sqrt.h"   /* provides floor_sqrt(seq_t) */

seq_t a(idx_t n)
{
    /* a(0) is defined as 0 */
    if (n == (idx_t)0)
        return (seq_t)0;

    /* Compute n*n*(n-1) safely as seq_t values */
    seq_t nn = (seq_t)n * (seq_t)n;          /* n^2   */
    seq_t nm1 = (seq_t)(n - (idx_t)1);      /* n-1   */
    seq_t prod = nn * nm1;                  /* n^2*(n-1) */

    /* floor(n*sqrt(n-1)/2) = floor(floor_sqrt(prod)/2) */
    seq_t sqrt_prod = floor_sqrt(prod);
    seq_t half = sqrt_prod / (seq_t)2;

    /* a(n) = n + floor(n*sqrt(n-1)/2) */
    return (seq_t)n + half;
}
