/*
 * A332699  First row of A332662, also main diagonal of A332667.
 * https://oeis.org/A332699
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n + C(k+2,3), where * k = floor((sqrt(8*n+1) − 1)/2) *
 *            C(m,3) = m*(m−1)*(m−2)/6 (triangular‐cube formula)
 * Range:     u8 n <= 7, u16 n <= 58, u32 n <= 58, u64 n <= 58, u128 n <= 58,
 *            big any n
 * Validated: partial, pass, 59 terms through n = 58 (2026-11-01)
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
    seq_t nn  = n;                         /* convert index to seq_t   */
    seq_t t   = nn * 8 + 1;                /* 8*n + 1                  */
    seq_t root = floor_sqrt(t);             /* floor(sqrt(8*n+1))      */
    seq_t k   = (root - 1) / 2;            /* largest k with k(k+1)/2 ≤ n */

    /* compute C(k+2,3) = k*(k+1)*(k+2)/6  (no overflow in the chosen order) */
    seq_t s = k * (k + 1);                 /* k*(k+1)   */
    s = s / 2;                             /* divide by 2 (always even) */
    s = s * (k + 2);                       /* (k*(k+1)/2)*(k+2) */
    s = s / 3;                             /* divide by 3 (always divisible) */

    return nn + s;                         /* final value of a(n)      */
}
