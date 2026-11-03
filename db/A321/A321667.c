/*
 * A321667  (3/2) * n^(2/3) rounded to nearest integer.
 * https://oeis.org/A321667
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = round((3/2) * n^(2/3))
 * Range:     u8 n <= 1, u16 n <= 34, u32 n <= 5345, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 91 terms through n = 9428 (2026-11-01)
 * Helpers:   ipow_i32
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/ipow_i32.h"

/* Floor cube root by binary search */
static inline seq_t floor_cube_root(seq_t x)
{
    seq_t lo = 0, hi = 1;
    /* raise hi until hi^3 > x */
    while (ipow_i32(hi, 3) <= x)
        hi <<= 1;
    while (lo < hi)
    {
        seq_t mid = (lo + hi) >> 1;
        seq_t mid3 = ipow_i32(mid, 3);
        if (mid3 <= x)
            lo = mid + 1;
        else
            hi = mid;
    }
    return lo - 1;
}

seq_t a(idx_t n)
{
    /* If the index is 0, return 0 (out of the OEIS range). */
    if (n == 0) return 0;

    /* Compute n^2 as a seq_t. */
    seq_t nn = (seq_t)n * (seq_t)n;      /* n^2 */

    /* lhs = 27 * n^2  (equivalent to (3 * n^(2/3))^3). */
    seq_t lhs = nn * 27;

    /* y = floor_cube_root(lhs) = floor(3 * n^(2/3)). */
    seq_t y = floor_cube_root(lhs);

    /* Floor of (3/2) * n^(2/3). */
    seq_t a_floor = y / 2;               /* integer division */

    /* Determine if we should round up. */
    seq_t r = a_floor * 2 + 1;           /* 2*a_floor + 1 */
    seq_t r3 = r * r * r;                /* (2*a_floor + 1)^3 */

    if (lhs >= r3)
        return a_floor + 1;              /* round up */
    else
        return a_floor;                  /* keep floor value */
}
