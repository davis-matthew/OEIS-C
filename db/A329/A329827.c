/*
 * A329827  Beatty sequence for (5+sqrt(37))/6.
 * https://oeis.org/A329827
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = floor( n * (5 + sqrt(37)) / 6 )
 * Range:     u8 n <= 1, u16 n <= 21, u32 n <= 63, u64 n <= 63, u128 n <= 63,
 *            big any n
 * Validated: partial, pass, 63 terms through n = 63 (2026-11-01)
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
    /* convert index to seq_t for arithmetic */
    seq_t k = (seq_t)n;

    /* compute floor(k * sqrt(37)) as floor(sqrt(37 * k^2)) */
    seq_t sq = (seq_t)37 * k * k;
    seq_t s = floor_sqrt(sq);

    /* compute floor((5*k + s)/6) which equals floor(n*r) */
    seq_t num = (seq_t)5 * k + s;
    seq_t res = num / (seq_t)6;

    return res;
}
