/*
 * A346693  Minimum integer length of a segment that touches the interior of n
 *          squares on a unit square grid.
 * https://oeis.org/A346693
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 1 for n <= 3; a(n) = ceil(sqrt((n-3)^2/2 + 1)) for n >= 4
 * Range:     u8 n <= 10, u16 n <= 34, u32 n <= 70, u64 n <= 70, u128 n <= 70,
 *            big any n
 * Helpers:   isqrt_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/isqrt_seq.h"

seq_t a(idx_t n)
{
    if (n <= 3) return 1;

    seq_t m = (seq_t)(n - 3);       /* (n-3)          */
    seq_t e = m * m;                /* (n-3)^2        */
    seq_t target = e + 2;           /* e + 2          */

    /* We need the smallest s such that 2*s^2 >= target.
       Start with floor_sqrt(target/2).                     */
    seq_t s = isqrt_seq(target >> 1);
    while ((seq_t)2 * s * s < target) ++s; /* at most one increment */
    return s;
}
