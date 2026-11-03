/*
 * A361197  a(n) is the number of equations in the set {x^2 + 2y^2 = n, 2x^2 +
 *          3y^2 = n, ..., k*x^2 + (k+1)*y^2 = n, ..., n*x^2 + (n+1)*y^2 = n}
 *          which admit at least one nonnegative integer solution.
 * https://oeis.org/A361197
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = |{ k | 1 ≤ k ≤ n and ∃ x,y ≥ 0 with k·x² + (k+1)·y² = n
 *            }|
 * Range:     u8 n <= 63, u16 n <= 96, u32 n <= 96, u64 n <= 96, u128 n <= 96,
 *            big any n
 * Validated: partial, pass, 96 terms through n = 96 (2026-11-01)
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

/* Test whether v is a perfect square using floor_sqrt. */
static inline bool is_square(seq_t v)
{
    seq_t r = floor_sqrt(v);
    return r * r == v;
}

/* Return the number of equations k*x^2 + (k+1)*y^2 = n
   that admit at least one non‑negative integer solution. */
seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* safety for n < 1 */
    seq_t N = (seq_t)n;
    seq_t count = 0;

    for (idx_t k = 1; k <= n; ++k) {
        seq_t kk = (seq_t)k;
        seq_t x_max = floor_sqrt(N / kk);

        bool found = false;
        for (seq_t x = 0; x <= x_max; ++x) {
            seq_t prod = kk * (x * x);
            seq_t rem  = N - prod;
            seq_t denom = kk + 1;

            if (rem % denom) continue;

            seq_t y2 = rem / denom;
            if (is_square(y2)) {
                found = true;
                break;
            }
        }
        if (found) ++count;
    }
    return count;
}
