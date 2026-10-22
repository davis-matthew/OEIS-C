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
 * Helpers:   floor_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/floor_sqrt.h"   /* provides seq_t floor_sqrt(seq_t) */

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
