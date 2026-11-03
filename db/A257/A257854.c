/*
 * A257854  a(n) = 2*n^4 - floor(2^(1/4)*n)^4.
 * https://oeis.org/A257854
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2*n^4 - floor(2^(1/4)*n)^4
 * Range:     u8 n <= 2, u16 n <= 9, u32 n <= 40, u64 n <= 40, u128 n <= 40,
 *            big any n
 * Validated: partial, pass, 41 terms through n = 40 (2026-11-01)
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
    seq_t n_seq = (seq_t)n;
    seq_t n4 = ipow_i32(n_seq, 4);          /* n^4 */
    seq_t two_n4 = n4 + n4;                /* 2*n^4 */
    seq_t sqrt1 = floor_sqrt(two_n4);     /* floor(sqrt(2*n^4)) */
    seq_t sqrt2 = floor_sqrt(sqrt1);      /* floor(sqrt(floor(sqrt(2*n^4)))) */
    seq_t k = sqrt2;                      /* floor(2^(1/4)*n) */
    seq_t k4 = ipow_i32(k, 4);             /* k^4 */
    return two_n4 - k4;                   /* 2*n^4 - k^4 */
}
