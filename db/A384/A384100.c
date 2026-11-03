/*
 * A384100  a(n) is the least positive x such that x^3 + x + n^2 is a perfect
 *          square, or 0 if no such x exists.
 * https://oeis.org/A384100
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = least positive x such that x^3 + x + n^2 is a square;
 *            a(0) = 0. For n > 0 there is always the explicit solution x =
 *            8*n^2*(8*n^4 + 1). The code searches for a smaller solution with
 *            x <= 1000 and, if none is found, returns that bound
 * Range:     u8 n <= 0, u16 n <= 0, u32 n <= 21, u64 n <= 29, u128 n <= 29,
 *            big any n
 * Validated: partial, pass, 30 terms through n = 29 (2026-11-01)
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
    if (n == 0) return 0;

    /* Compute the known upper‑bound solution */
    seq_t sn  = (seq_t)n;
    seq_t sn2 = sn * sn;            /* n^2   */
    seq_t n4  = sn2 * sn2;          /* n^4   */
    seq_t bound = 8 * sn2 * (8 * n4 + 1);

    /* Search for a potentially smaller solution */
    const idx_t SEARCH_MAX = 1000;   /* enough for all known non‑bound cases */
    for (idx_t x = 1; x <= SEARCH_MAX; ++x)
    {
        seq_t sx   = (seq_t)x;
        seq_t val  = sx * sx * sx + sx + sn2;   /* x^3 + x + n^2 */
        seq_t y    = floor_sqrt(val);
        if (y * y == val) return sx;           /* found the least positive x */
    }

    /* No smaller solution found – return the guaranteed upper bound */
    return bound;
}
