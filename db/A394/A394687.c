/*
 * A394687  Number of ordered pairs of positive integers (a,b) with a < b such
 *          that b^2 - a^2 <= n.
 * https://oeis.org/A394687
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Sum_{k=1..floor((n-1)/2)} ( floor( sqrt(k^2 + n) ) - k )
 *            where sqrt() is the integer square‑root (floor)
 * Range:     u8 n <= 14, u16 n <= 100, u32 n <= 100, u64 n <= 100,
 *            u128 n <= 100, big any n
 * Validated: partial, pass, 100 terms through n = 100 (2026-11-01)
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
    seq_t result = 0;
    idx_t limit = (n - 1) / 2;          /* last k to consider */
    for (idx_t k = 1; k <= limit; ++k) {
        seq_t kk  = (seq_t)k * (seq_t)k;   /* k^2 */
        seq_t val = kk + (seq_t)n;         /* k^2 + n */
        seq_t s   = floor_sqrt(val);       /* floor sqrt(k^2 + n) */
        result += s - (seq_t)k;            /* add contribution for this k */
    }
    return result;
}
