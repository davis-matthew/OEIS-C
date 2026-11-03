/*
 * A351846  Irregular triangle read by rows: T(n,k), n >= 0, k >= 0, in which
 *          n appears 4*n + 1 times in row n.
 * https://oeis.org/A351846
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabf, easy
 * Formula:   a(n) = floor((sqrt(8*n + 1) + 1)/4)
 * Range:     u8 n <= 7, u16 n <= 90, u32 n <= 90, u64 n <= 90, u128 n <= 90,
 *            big any n
 * Validated: partial, pass, 91 terms through n = 90 (2026-11-01)
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
    /* Convert index to seq_t for arithmetic */
    seq_t val = (seq_t)n;
    /* Compute 8*n + 1 in seq_t, safe for 64/128 bits */
    seq_t x = val * 8 + 1;
    /* Integer square root of x */
    seq_t s = floor_sqrt(x);
    /* floor((s + 1)/4) */
    return (s + 1) / 4;
}
