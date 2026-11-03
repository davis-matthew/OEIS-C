/*
 * A334347  Number of r X s rectangles such that r + s = 2n, where exactly one
 *          of r or s is a positive square.
 * https://oeis.org/A334347
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = number of unordered rectangle pairs (r, s) with r + s =
 *            2n where exactly one of r or s is a perfect square. The
 *            computation is O(√n)
 * Range:     u8 n <= 32, u16 n <= 80, u32 n <= 80, u64 n <= 80, u128 n <= 80,
 *            big any n
 * Validated: partial, pass, 80 terms through n = 80 (2026-11-01)
 * Helpers:   ipow_i32
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
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

static inline bool is_square(seq_t x)
{
    seq_t r = floor_sqrt(x);
    return r * r == x;
}

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;          /* not part of the offset, just safe */

    /* two_n = 2 * n */
    seq_t two_n = (seq_t)n + (seq_t)n;
    seq_t limit = two_n - 1;              /* largest square to consider */

    seq_t count = (seq_t)0;

    for (idx_t k = 1;; ++k) {
        seq_t ksq = (seq_t)k * (seq_t)k;
        if (ksq > limit) break;

        seq_t rem = two_n - ksq;           /* other side of the rectangle */
        if (is_square(rem)) continue;     /* skip if both sides are squares */

        ++count;
    }
    return count;
}
