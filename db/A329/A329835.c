/*
 * A329835  Beatty sequence for (9+sqrt(101))/10.
 * https://oeis.org/A329835
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = floor( (9*n + floor(n*sqrt(101)) ) / 10 )
 * Range:     u8 none, u16 n <= 12, u32 n <= 63, u64 n <= 63, u128 n <= 63,
 *            big any n
 * Validated: partial, pass, 63 terms through n = 63 (2026-11-01)
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
    /* Compute floor(n*sqrt(101)) as floor(sqrt(101*n^2)) */
    seq_t n_seq   = (seq_t)n;
    seq_t nsq     = n_seq * n_seq;             /* n^2    */
    seq_t prod    = nsq * (seq_t)101;          /* 101*n^2 */
    seq_t sqrtv   = floor_sqrt(prod);          /* floor(sqrt(101*n^2)) = floor(n*sqrt(101)) */

    seq_t nine_n  = (seq_t)9 * n_seq;
    seq_t numer   = nine_n + sqrtv;            /* 9*n + floor(n*sqrt(101)) */
    seq_t result  = numer / (seq_t)10;         /* floor((9*n + floor(n*sqrt(101))) / 10) */

    return result;
}
