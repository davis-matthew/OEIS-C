/*
 * A308302  Number of integer-sided triangles with perimeter n and at least
 *          one nonsquare side length.
 * https://oeis.org/A308302
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 63, u16 n <= 69, u32 n <= 69, u64 n <= 69, u128 n <= 69,
 *            big any n
 * Validated: partial, pass, 69 terms through n = 69 (2026-11-01)
 * Helpers:   ipow_i32
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A308302: Number of integer‑sided triangles with perimeter n
   and at least one side that is not a perfect square.
   The value is A005044(n) – A308064(n). */

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

/* Test whether a seq_t number is a perfect square. */
static inline bool is_perfect_square(seq_t x)
{
    seq_t r = floor_sqrt(x);
    return r * r == x;
}

/* External linkage as required. */
seq_t a(idx_t n)
{
    if (n < 3) return 0;

    seq_t total   = 0;   /* Total integer triangles (unordered). */
    seq_t squares = 0;   /* Triangles whose all three sides are perfect squares. */

    /* ---- Count all integer‑sided triangles (a ≤ b ≤ c) ---- */
    for (idx_t a = 1; a <= n / 3; ++a) {
        idx_t max_b = (n - a) / 2;           /* ensures b ≤ c. */
        for (idx_t b = a; b <= max_b; ++b) {
            idx_t c = n - a - b;
            if ((seq_t)(a + b) > (seq_t)c)   /* triangle inequality, strict. */
                ++total;
        }
    }

    /* ---- Count triangles with all sides perfect squares ---- */
    idx_t max_sq_root = (idx_t)floor_sqrt((seq_t)n);
    for (idx_t a_sq = 1; a_sq <= max_sq_root; ++a_sq) {
        idx_t a = a_sq * a_sq;
        if (a > n) break;                   /* safety, though not reached. */

        for (idx_t b_sq = a_sq; b_sq <= max_sq_root; ++b_sq) {
            idx_t b = b_sq * b_sq;
            if (a + b > n) break;            /* c would be negative. */

            idx_t c = n - a - b;
            if (c < b || c == 0) continue;   /* maintain order, reject zero. */

            if (!is_perfect_square(c)) continue;     /* require c a square. */

            /* triangle inequality is automatically satisfied because
               a + b = n - c > c when c is the largest side and a,b non‑negative. */
            if ((seq_t)(a + b) > (seq_t)c)
                ++squares;
        }
    }

    return total - squares;
}
