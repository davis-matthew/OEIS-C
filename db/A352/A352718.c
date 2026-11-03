/*
 * A352718  a(n) = floor(n^(3/2)) - floor(n^(1/2))^3.
 * https://oeis.org/A352718
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = floor(n^(3/2)) - floor(sqrt(n))^3
 * Range:     u8 n <= 3, u16 n <= 25, u32 n <= 66, u64 n <= 66, u128 n <= 66,
 *            big any n
 * Validated: partial, pass, 67 terms through n = 66 (2026-11-01)
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
    seq_t sn  = (seq_t)n;          /* convert index to seq_t */
    seq_t m   = floor_sqrt(sn);    /* m = floor(sqrt(n)) */
    seq_t m3  = m * m * m;         /* m^3 */
    seq_t n3  = sn * sn * sn;      /* n^3 */
    seq_t ns3 = floor_sqrt(n3);    /* floor(sqrt(n^3)) = floor(n^(3/2)) */
    return ns3 - m3;               /* a(n) */
}
