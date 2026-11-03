/*
 * A276865  First differences of the Beatty sequence A003512 for 2 + sqrt(3).
 * https://oeis.org/A276865
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = floor(n*(2+sqrt(3))) - floor((n-1)*(2+sqrt(3))) = 2 +
 *            floor(sqrt(3*n^2)) - floor(sqrt(3*(n-1)^2)) (here sqrt() denotes
 *            the floor of the square root)
 * Range:     u8 n <= 4, u16 n <= 73, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 102 terms through n = 9608 (2026-11-01)
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

seq_t a(idx_t n) {
    seq_t nseq   = (seq_t)n;
    seq_t n1seq  = (seq_t)(n - 1);

    /* floor(sqrt(3*n^2)) */
    seq_t sq_cur = floor_sqrt((seq_t)3 * nseq * nseq);

    /* floor(sqrt(3*(n-1)^2)) */
    seq_t sq_prev = floor_sqrt((seq_t)3 * n1seq * n1seq);

    return (seq_t)2 + sq_cur - sq_prev;
}
