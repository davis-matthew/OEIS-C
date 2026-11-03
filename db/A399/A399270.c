/*
 * A399270  Number of nonnegative integer solutions to the equation x^2 +
 *          9*y^2 = n.
 * https://oeis.org/A399270
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = #{(x,y) ∈ ℕ₀² : x² + 9·y² = n}
 * Range:     u8 n <= 63, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 117 terms through n = 9922 (2026-11-01)
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

seq_t a(idx_t n)
{
    seq_t count = 0;
    seq_t N = (seq_t)n;
    for (idx_t y = 0; ; ++y)
    {
        seq_t term = (seq_t)9 * (seq_t)y * (seq_t)y;
        if (term > N) break;
        seq_t r = N - term;
        seq_t x = floor_sqrt(r);
        if (x * x == r) count++;
    }
    return count;
}
