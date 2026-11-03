/*
 * A347594  a(0) = 1; for n>0, a(n) is the smallest positive integer such that
 *          a(n-1)^2 + n^2 + a(n) is a square.
 * https://oeis.org/A347594
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, look
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 27, u32 n <= 227, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 80 terms through n = 8074 (2026-11-01)
 * Helpers:   ipow_i32
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * A347594:  a(0) = 1; for n>0, a(n) is the smallest positive integer such that
 * a(n‑1)^2 + n^2 + a(n) is a perfect square.
 * Recurrence implementation:
 *   let T = a(n‑1)^2 + n^2
 *   let q = ⌊√T⌋
 *   a(n) = (q+1)^2 – T
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

/* Compute a(n) by direct iteration.  No caching is required for the
 * given limits (n up to ~10 000) and avoids the trouble of allocating
 * memory for arbitrary‑precision types. */
seq_t a(idx_t n)
{
    seq_t prev = 1;                 /* a(0) */
    if (n == 0) return prev;

    for (idx_t i = 1; i <= n; ++i) {
        seq_t ii   = (seq_t)i;
        seq_t t    = prev * prev + ii * ii;       /* a(n-1)^2 + n^2 */
        seq_t q    = floor_sqrt(t);               /* ⌊√T⌋ */
        seq_t m    = q + 1;                       /* next integer ≥ √T */
        seq_t m2   = m * m;                       /* m^2 */
        seq_t val  = m2 - t;                      /* a(n) = m^2 – T */
        prev = val;
    }
    return prev;
}
