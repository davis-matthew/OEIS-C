/*
 * A329841  Beatty sequence for (7+sqrt(109))/10.
 * https://oeis.org/A329841
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = floor((7*n + floor(n*sqrt(109)))/10)
 * Range:     u8 none, u16 n <= 12, u32 n <= 64, u64 n <= 64, u128 n <= 64,
 *            big any n
 * Validated: partial, pass, 64 terms through n = 64 (2026-11-01)
 * Helpers:   ipow_i32
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*   r = (7 + sqrt(109)) / 10, a(n) = floor(n*r)   */
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
    /* Problem statement guarantees n ≥ 1; 0 is handled harmlessly. */
    if (n == 0)
        return 0;

    seq_t nn = (seq_t)n;            /* convert index to seq_t */
    seq_t sq = nn * nn;             /* n^2  */
    seq_t part = sq * (seq_t)109;   /* 109 * n^2  */
    seq_t sqrt_val = floor_sqrt(part);      /* floor(n * sqrt(109))  */
    return (nn * (seq_t)7 + sqrt_val) / (seq_t)10;   /* floor((7n + sqrt_val)/10) */
}
