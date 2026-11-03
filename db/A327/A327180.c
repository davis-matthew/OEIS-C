/*
 * A327180  a(n) = [(2n+1)r] - [(n+1)r] - [nr], where [ ] = floor and r =
 *          sqrt(3).
 * https://oeis.org/A327180
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = floor((2n+1)·sqrt(3)) – floor((n+1)·sqrt(3)) –
 *            floor(n·sqrt(3));
 * Range:     u8 n <= 1, u16 n <= 36, u32 n <= 7930, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 102 terms through n = 9994 (2026-11-01)
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
    /* cast n to seq_t only once */
    seq_t n_seq = (seq_t)n;

    /* compute the three arguments */
    seq_t k1 = 2 * n_seq + 1;   /* 2n+1 */
    seq_t k2 = n_seq + 1;       /* n+1 */
    seq_t k3 = n_seq;           /* n */

    /* floor(sqrt(3)*k) = floor(sqrt(3*k*k)) for integer k */
    seq_t f1 = floor_sqrt((seq_t)3 * k1 * k1);
    seq_t f2 = floor_sqrt((seq_t)3 * k2 * k2);
    seq_t f3 = floor_sqrt((seq_t)3 * k3 * k3);

    /* final result */
    return f1 - f2 - f3;
}
