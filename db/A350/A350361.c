/*
 * A350361  2-tone chromatic number of a tree with maximum degree n.
 * https://oeis.org/A350361
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = ceil((5 + sqrt(1 + 8*n))/2)
 * Range:     u8 n <= 7, u16 n <= 71, u32 n <= 71, u64 n <= 71, u128 n <= 71,
 *            big any n
 * Validated: partial, pass, 71 terms through n = 71 (2026-11-01)
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
    /* n ≥ 1 by definition of the offset; use the formula directly */
    seq_t X = (seq_t)1 + (seq_t)8 * (seq_t)n;          /* 1 + 8*n   */
    seq_t s = floor_sqrt(X);                          /* floor(sqrt(1+8*n)) */

    seq_t sum   = (seq_t)5 + s;                       /* 5 + s     */
    seq_t base  = sum >> 1;                            /* floor((5+s)/2)   */
    seq_t rem   = sum & 1;                             /* (5+s) mod 2      */

    /* If sqrt(1+8*n) is exact, use ceil((5+s)/2); otherwise add 1
       because ceil((5+exact)/2) = floor((5+s)/2) + (5+s mod 2),
       and ceil((5+inexact)/2) = floor((5+s)/2) + 1. */
    if (s * s == X)                                     /* exact sqrt ? */
        return base + rem;                               /* ceil for integer sqrt */
    else
        return base + 1;                                 /* ceil for non‑integer sqrt */
}
