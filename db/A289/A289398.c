/*
 * A289398  Least integer m > n such that (n^2 + m^2)/2 is a square.
 * https://oeis.org/A289398
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, look
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 25, u32 n <= 5785, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 76 terms through n = 8875 (2026-11-01)
 * Helpers:   ipow_i32
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Find the least integer m > n such that (n^2 + m^2)/2 is a perfect square. */
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

/* Check if a seq_t number is a perfect square. */
static inline bool is_square(seq_t x)
{
    seq_t s = floor_sqrt(x);
    return s * s == x;
}

/* Main sequence function. */
seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* undefined below offset, but safe */
    seq_t n_sq = (seq_t)n * (seq_t)n;
    /* Upper bound: proven that a(n) <= 7*n for all n. */
    idx_t start = n + 1;
    idx_t limit = 7 * n;
    for (idx_t m = start; m <= limit; ++m) {
        seq_t m_sq = (seq_t)m * (seq_t)m;
        seq_t sum  = n_sq + m_sq;
        if ((sum & 1) != 0) continue;    /* sum must be even */
        seq_t t = sum >> 1;
        if (is_square(t))
            return (seq_t)m;
    }
    /* If not found within the proven bound, continue searching unbounded. */
    for (idx_t m = limit + 1;; ++m) {
        seq_t m_sq = (seq_t)m * (seq_t)m;
        seq_t sum  = n_sq + m_sq;
        if ((sum & 1) != 0) continue;
        seq_t t = sum >> 1;
        if (is_square(t))
            return (seq_t)m;
    }
}
