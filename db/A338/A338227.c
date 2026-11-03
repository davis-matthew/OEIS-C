/*
 * A338227  a(n) = x(n) mod floor(sqrt(x(n))), where x(n) = floor((n^2)/2).
 * https://oeis.org/A338227
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = floor(n^2/2) mod floor(sqrt(floor(n^2/2)))
 * Range:     u8 n <= 11, u16 n <= 75, u32 n <= 75, u64 n <= 75, u128 n <= 75,
 *            big any n
 * Validated: partial, pass, 74 terms through n = 75 (2026-11-01)
 * Helpers:   ipow_i32
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

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
    /* n ≥ 2 for the sequence, but handle smaller values safely */
    if (n < 2) return 0;

    /* x(n) = floor(n^2/2) */
    seq_t x = (seq_t)n * (seq_t)n / (seq_t)2;

    /* d(n) = floor(sqrt(x(n))) */
    seq_t d = floor_sqrt(x);

    /* For n ≥ 2, d ≥ 1, so modulo is well‑defined */
    return x % d;
}
