/*
 * A394684  Number of ordered pairs of integers (x,y) with 0 <= x <= y and
 *          x^2+y^2 < n^2.
 * https://oeis.org/A394684
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = # { (x,y) | 0 <= x <= y, x^2 + y^2 < n^2 }
 * Range:     u8 n <= 8, u16 n <= 65, u32 n <= 65, u64 n <= 65, u128 n <= 65,
 *            big any n
 * Validated: partial, pass, 66 terms through n = 65 (2026-11-01)
 * Helpers:   ipow_i32
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
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
    seq_t total = 0;
    seq_t n2 = (seq_t)n * (seq_t)n;

    for (idx_t x = 0; x < n; ++x) {
        seq_t x2 = (seq_t)x * (seq_t)x;
        seq_t rem = n2 - x2;          /* n^2 - x^2, positive while x < n */

        if (rem == 0) continue;       /* no y satisfies y^2 < 0 */
        seq_t ymax = floor_sqrt(rem - 1);  /* floor(sqrt(rem−1)) */

        seq_t sx = (seq_t)x;
        if (ymax < sx) continue;      /* y ≥ x condition fails */

        total += (ymax - sx + 1);     /* count y = x … ymax */
    }

    return total;
}
