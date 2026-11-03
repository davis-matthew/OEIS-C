/*
 * A276858  First differences of the Beatty sequence A022842 for sqrt(8).
 * https://oeis.org/A276858
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = floor(n*sqrt(8)) - floor((n-1)*sqrt(8))
 * Range:     u8 n <= 2, u16 n <= 45, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 102 terms through n = 9990 (2026-11-01)
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
    if (n == 0) return 0;  /* offset 1; return 0 for n=0 as a fallback */

    seq_t nn   = (seq_t)n;
    seq_t floor_n = floor_sqrt((seq_t)8 * nn * nn);

    seq_t floor_n0;
    if (n == 1) {
        floor_n0 = 0;
    } else {
        seq_t nn0 = (seq_t)(n - 1U);
        floor_n0 = floor_sqrt((seq_t)8 * nn0 * nn0);
    }

    return floor_n - floor_n0;
}
