/*
 * A329844  Beatty sequence for (11+sqrt(61))/6.
 * https://oeis.org/A329844
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = floor(n*(11 + sqrt(61))/6)
 * Range:     u8 n <= 1, u16 n <= 16, u32 n <= 58, u64 n <= 58, u128 n <= 58,
 *            big any n
 * Helpers:   floor_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/floor_sqrt.h"

seq_t a(idx_t n)
{
    /* For n = 0 this is outside the OEIS offset; return 0. */
    if (n == 0) return 0;

    /* Compute floor(n * sqrt(61)) as floor_sqrt(61 * n^2). */
    seq_t nsq = (seq_t)n * (seq_t)n;        /* n^2 */
    seq_t v   = nsq * (seq_t)61;            /* 61 * n^2 */
    seq_t sq  = floor_sqrt(v);              /* floor(n*sqrt(61)) */

    /* a(n) = floor((11*n + floor(n*sqrt(61))) / 6) */
    seq_t res = ((seq_t)11 * (seq_t)n + sq) / 6;
    return res;
}
