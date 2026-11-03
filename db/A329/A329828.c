/*
 * A329828  Beatty sequence for (7+sqrt(37))/6.
 * https://oeis.org/A329828
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = floor(n*(7+sqrt(37))/6)
 * Range:     u8 n <= 1, u16 n <= 21, u32 n <= 61, u64 n <= 61, u128 n <= 61,
 *            big any n
 * Validated: partial, pass, 61 terms through n = 61 (2026-11-01)
 * Helpers:   ipow_i32
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Beatty sequence for s = (7+sqrt(37))/6 */

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
    /* Compute floor((7*n + floor(sqrt(37*n*n))) / 6) */
    seq_t sn = (seq_t)n;              /* cast index to seq_t */
    seq_t n2 = sn * sn;               /* n^2 */
    seq_t prod = (seq_t)37 * n2;      /* 37 * n^2 */
    seq_t k = floor_sqrt(prod);       /* floor(sqrt(37*n^2)) == floor(n*sqrt(37)) */
    seq_t sum = (seq_t)7 * sn + k;    /* 7*n + floor(n*sqrt(37)) */
    return sum / (seq_t)6;            /* floor divided by 6 */
}
