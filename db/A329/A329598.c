/*
 * A329598  Partial sums of the nontriangular numbers (A014132).
 * https://oeis.org/A329598
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Σ_{i=1}^{x} i − m(m+1)(m+2)/6 with x−m = n and m =
 *            ⌊(√(8x+1) − 1)/2⌋
 * Range:     u8 n <= 2, u16 n <= 52, u32 n <= 52, u64 n <= 52, u128 n <= 52,
 *            big any n
 * Validated: partial, pass, 52 terms through n = 52 (2026-11-01)
 * Helpers:   ipow_i32
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Uses the integer square‑root routine from the utils library. */
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
    if (n == 0)
        return 0;

    seq_t sn = (seq_t)n;              /* convert index to seq_t for arithmetic */

    /* Find the smallest x such that the count of non‑triangular numbers <= x is ≥ n */
    idx_t lo = 1;
    idx_t hi = 2 * n + 10;            /* upper bound: x is close to n + √(2n) */

    while (lo < hi) {
        idx_t mid = lo + ((hi - lo) >> 1);
        seq_t mid_seq = (seq_t)mid;

        seq_t tmp = (seq_t)8 * mid_seq + 1;
        seq_t s   = floor_sqrt(tmp);
        seq_t m   = (s - 1) >> 1;     /* m = floor((√(8x+1)−1)/2) */

        seq_t cnt = mid_seq - m;      /* number of non‑triangular numbers <= mid */
        if (cnt >= sn)
            hi = mid;
        else
            lo = mid + 1;
    }

    seq_t x = (seq_t)lo;

    /* Sum of all numbers <= x */
    seq_t total = x * (x + 1) / 2;

    /* Sum of the triangular numbers <= x */
    seq_t tmp  = (seq_t)8 * x + 1;
    seq_t s    = floor_sqrt(tmp);
    seq_t m    = (s - 1) >> 1;
    seq_t sum_tri = m * (m + 1) * (m + 2) / 6;

    /* Desired result: sum of the first n non‑triangular numbers */
    return total - sum_tri;
}
