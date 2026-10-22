/*
 * A329832  Beatty sequence for (9+sqrt(65))/8.
 * https://oeis.org/A329832
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = floor(n*(9+sqrt(65))/8)
 * Range:     u8 none, u16 n <= 15, u32 n <= 62, u64 n <= 62, u128 n <= 62,
 *            big any n
 * Helpers:   floor_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/floor_sqrt.h"

seq_t a(idx_t n)
{
    if (n == 0)   /* Offset 1: a(1) is the first term; a(0) is unused */
        return 0;

    /* Compute floor(n * sqrt(65)) as floor_sqrt(65*n^2) */
    seq_t nn   = (seq_t)n * n;          /* n^2  */
    seq_t sq   = floor_sqrt(nn * 65);   /* floor(n * sqrt(65)) */

    /* Numerator: 9*n + floor(n * sqrt(65)) */
    seq_t num  = (seq_t)9 * n + sq;

    /* Final result: floor((9*n + floor(n*sqrt(65)))/8) */
    return num / 8;
}
