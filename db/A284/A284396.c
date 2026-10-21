/*
 * A284396  Positions of 2 in A284394.
 * https://oeis.org/A284396
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 3*floor(n*phi) + 2, phi = (1 + sqrt(5))/2
 * Range:     u8 n <= 3, u16 n <= 56, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   floor_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Implementation uses integer arithmetic only:
   floor(n*phi) = floor((n + floor(n*sqrt(5))) / 2),
   and floor(n*sqrt(5)) = floor(sqrt(5*n^2)).  */
#include "../utils/floor_sqrt.h"

seq_t a(idx_t n)
{
    /* Convert index to seq_t */
    seq_t sn = (seq_t)n;

    /* Compute sq = 5 * n * n.  Using multiplication in two steps
       avoids intermediate overflow in 64‑bit builds when possible. */
    seq_t sq = sn * sn;
    sq = sq * (seq_t)5;

    /* s = floor(n * sqrt(5)) = floor(sqrt(5*n^2)) */
    seq_t s = floor_sqrt(sq);

    /* t = floor((n + s) / 2) = floor(n*phi) */
    seq_t t = (sn + s) / (seq_t)2;

    /* a(n) = 3*t + 2 */
    return (seq_t)3 * t + (seq_t)2;
}
