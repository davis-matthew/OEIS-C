/*
 * A329846  Beatty sequence for (7+sqrt(29))/5.
 * https://oeis.org/A329846
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = floor(n*(7+sqrt(29))/5)
 * Range:     u8 n <= 1, u16 n <= 23, u32 n <= 60, u64 n <= 60, u128 n <= 60,
 *            big any n
 * Helpers:   floor_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/floor_sqrt.h"

seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* undefined below offset, guard */
    seq_t nn = (seq_t)n;
    seq_t prod = nn * nn;          /* n^2 */
    prod *= 29;                    /* 29*n^2 */
    seq_t t = floor_sqrt(prod);    /* floor(n * sqrt(29)) */
    seq_t res = (7 * nn + t) / 5;  /* floor((7n + t)/5) */
    return res;
}
