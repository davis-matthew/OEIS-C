/*
 * A329837  Beatty sequence for (4+sqrt(26))/5.
 * https://oeis.org/A329837
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = floor(n*(4 + sqrt(26)) / 5)
 * Range:     u8 n <= 1, u16 n <= 25, u32 n <= 63, u64 n <= 63, u128 n <= 63,
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

seq_t a(idx_t n) {
    /* Handle the offset: for n == 0 we return 0, though the sequence
       starts at n = 1. This guards against accidental use with n == 0. */
    if (n == 0) return 0;

    seq_t nn = (seq_t)n;
    /* Compute floor(n * sqrt(26)) as floor(sqrt(26 * n^2)) */
    seq_t sq = nn * nn;
    sq *= 26;
    seq_t s = floor_sqrt(sq);

    /* a(n) = floor((4n + floor(n*sqrt(26))) / 5) */
    return (4 * nn + s) / 5;
}
