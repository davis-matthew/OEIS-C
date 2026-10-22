/*
 * A329833  Beatty sequence for (5+sqrt(73))/8.
 * https://oeis.org/A329833
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = floor(n*(5+sqrt(73))/8)
 * Range:     u8 none, u16 n <= 14, u32 n <= 64, u64 n <= 64, u128 n <= 64,
 *            big any n
 * Helpers:   floor_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/floor_sqrt.h"

seq_t a(idx_t n)
{
    /* Convert index to seq_t */
    seq_t big_n = (seq_t)n;

    /* Compute floor(n * sqrt(73)) as floor(sqrt(73 * n^2)) */
    seq_t sq = big_n * big_n;               /* n^2          */
    seq_t arg = 73 * sq;                    /* 73 * n^2     */
    seq_t sqrt_floor = floor_sqrt(arg);     /* floor(sqrt)  */

    /* Compute floor((5n + floor(n*sqrt73))/8) */
    seq_t numerator = 5 * big_n + sqrt_floor;
    return numerator / 8;
}
