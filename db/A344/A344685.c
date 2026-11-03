/*
 * A344685  Triangle T(n, k) obtained from the array N1(a, b) = a^2 + a*b -
 *          b^2, for a >= 0 and b >= 0, read by upwards antidiagonals.
 * https://oeis.org/A344685
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, tabl, easy
 * Formula:   a(n) = r^2 - r·k - k^2 where r = ⌊(√(8n+1)-1)/2⌋ , k = n -
 *            r(r+1)/2 (indices start at 0)
 * Range:     i8 n <= 7, i16 n <= 65, i32 n <= 65, i64 n <= 65, i128 n <= 65,
 *            big any n
 * Validated: partial, pass, 66 terms through n = 65 (2026-11-01)
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
    /* Compute row index r from the position n in the flattened triangle. */
    seq_t rn   = 8 * (seq_t)n + 1;           /* 8*n + 1  */
    seq_t s    = floor_sqrt(rn);            /* floor(sqrt(8n+1)) */
    seq_t r    = (s - 1) / 2;                /* floor((sqrt(8n+1)-1)/2) */

    /* Column index k within row r. */
    seq_t tri  = r * (r + 1) / 2;            /* triangular number r(r+1)/2 */
    seq_t k    = (seq_t)n - tri;             /* k = n - tri */

    /* T(r,k) = r^2 - r*k - k^2 */
    return r * r - r * k - k * k;
}
