/*
 * A355159  Numbers k such that (fractional part of k^(3/2)) < 1/2.
 * https://oeis.org/A355159
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n-th integer k with fractional part of k^(3/2) < 1/2
 * Range:     u8 n <= 2, u16 n <= 16, u32 n <= 63, u64 n <= 63, u128 n <= 63,
 *            big any n
 * Validated: partial, pass, 64 terms through n = 63 (2026-11-01)
 * Helpers:   ipow_i32
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The condition is equivalent to: k^3 <= m^2 + m where m = floor_sqrt(k^3). */

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

static inline bool is_satisfying(seq_t k)
{
    seq_t k2 = k * k;          /* k^2  */
    seq_t k3 = k2 * k;         /* k^3  */
    seq_t m  = floor_sqrt(k3); /* ⌊√(k^3)⌋ */
    seq_t rhs = m * m + m;     /* m^2 + m */
    return k3 <= rhs;
}

seq_t a(idx_t n)
{
    idx_t count = 0;
    seq_t k = (seq_t)0;

    for (;; ++k) {
        if (is_satisfying(k)) {
            if (count == n) return k;
            ++count;
        }
    }
}
