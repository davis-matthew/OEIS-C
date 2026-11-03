/*
 * A306236  a(n) is the smallest integer m > n with integer j > m makes n^2,
 *          m^2 and j^2 an arithmetic progression.
 * https://oeis.org/A306236
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, look
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 18, u32 n <= 3832, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 74 terms through n = 8939 (2026-11-01)
 * Helpers:   ipow_i32
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Implementation of OEIS A306236:
   a(n) = smallest integer m > n such that ∃ integer j > m with
   n^2, m^2, j^2 in arithmetic progression.
   This is equivalent to finding the least m > n for which
   2*m^2 - n^2 is a perfect square > m^2. */
/* The property a(n) ≤ 5n holds for all n; we search up to 5n. */

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
    /* trivial for n = 0, unused in OEIS (offset 1) */
    if (n == 0) return 0;

    /* pre‑compute n^2 once */
    seq_t n_sq = (seq_t)n * (seq_t)n;

    idx_t maxm = 5 * n;                /* a(n) ≤ 5n */
    idx_t m = n + 1;

    /* a(n) has same parity as n, skip mismatched values */
    if (((m ^ n) & 1) != 0) ++m;       /* ensure m and n share parity */

    for (; m <= maxm; m += 2) {        /* step by 2 to keep parity */
        seq_t m_sq = (seq_t)m * (seq_t)m;
        seq_t t = 2 * m_sq - n_sq;     /* 2*m^2 - n^2 */

        seq_t r = floor_sqrt(t);      /* integer floor sqrt of t */

        if (r * r == t && r > (seq_t)m) {
            return (seq_t)m;          /* found the smallest m */
        }
    }

    /* In theory this point should never be reached because a(n) ≤ 5n;
       return 0 as a fall‑back. */
    return 0;
}
