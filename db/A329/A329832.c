/*
 * A329832  Beatty sequence for (9+sqrt(65))/8.
 * https://oeis.org/A329832
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = floor(n*(9+sqrt(65))/8)
 * Range:     u8 none, u16 n <= 15, u32 n <= 62, u64 n <= 62, u128 n <= 62,
 *            big any n
 * Validated: partial, pass, 62 terms through n = 62 (2026-11-01)
 * Helpers:   ipow_i32
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/ipow_i32.h"

/* Floor square root by binary search */
static inline seq_t floor_sqrt(seq_t x)
{
    seq_t lo = 0, hi = 1;
    while (ipow_i32(hi, 2) <= x)
        hi <<= 1;
    while (lo < hi)
    {
        seq_t mid = (lo + hi) >> 1;
        seq_t mid2 = mid * mid;
        if (mid2 <= x)
            lo = mid + 1;
        else
            hi = mid;
    }
    return lo - 1;
}

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
