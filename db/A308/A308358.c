/*
 * A308358  Beatty sequence for sqrt(3)/4.
 * https://oeis.org/A308358
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = floor(n*sqrt(3)/4)
 * Range:     u8 n <= 4, u16 n <= 73, u32 n <= 80, u64 n <= 80, u128 n <= 80,
 *            big any n
 * Validated: partial, pass, 81 terms through n = 80 (2026-11-01)
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
    /* Compute floor(n*sqrt(3)/4).  
       Since floor(n*sqrt(3)/4) = floor(floor(n*sqrt(3))/4) and
       floor(n*sqrt(3)) = floor(sqrt(3*n*n)), we first find the
       integer part of sqrt(3*n*n) and then divide by 4. */
    seq_t nn   = (seq_t)n;
    seq_t sq   = nn * nn;          /* n^2      */
    sq        = sq * (seq_t)3;     /* 3*n^2    */
    seq_t root = floor_sqrt(sq);   /* floor(sqrt(3*n^2)) */
    return root / 4;
}
