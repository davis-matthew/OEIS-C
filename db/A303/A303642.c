/*
 * A303642  a(n) is the number of lattice points in Cartesian grid between
 *          circle of radius n and its inscribed square. The sides of the
 *          square are parallel to coordinate axes.
 * https://oeis.org/A303642
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (#integer points with x² + y² < n²) – (2·⌊n/√2⌋ + 1)²
 * Range:     u8 n <= 8, u16 n <= 52, u32 n <= 52, u64 n <= 52, u128 n <= 52,
 *            big any n
 * Validated: partial, pass, 52 terms through n = 52 (2026-11-01)
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
    /* Edge case: n == 0 is not below the offset, but guard against underflow */
    if (n == 0) return 0;

    seq_t n2 = (seq_t)n * ((seq_t)n);          /* n² */

    /* Count lattice points inside the circle: x² + y² < n² */
    seq_t total = 0;
    for (idx_t x = 0; x < n; ++x)
    {
        seq_t x2 = (seq_t)x * ((seq_t)x);
        /* y² < n² – x²  -->  y_max = ⌊√(n² – x² – 1)⌋ */
        seq_t rem = n2 - x2 - 1;
        seq_t y_max = floor_sqrt(rem);
        seq_t count_x = 2 * y_max + 1;        /* including y = 0 */

        if (x == 0)
            total += count_x;                /* only the non‑negative side */
        else
            total += count_x * 2;            /* positive & negative x */
    }

    /* Count lattice points inside the inscribed square */
    seq_t t = floor_sqrt(n2 / 2);              /* ⌊n/√2⌋ */
    seq_t square_points = (2 * t + 1) * (2 * t + 1);

    return total - square_points;
}
