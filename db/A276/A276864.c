/*
 * A276864  First differences of the Beatty sequence A001952 for 2 + sqrt(2).
 * https://oeis.org/A276864
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2 + floor(n*sqrt(2)) - floor((n-1)*sqrt(2))
 * Range:     u8 n <= 5, u16 n <= 86, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Validated: standard, pass, 102 terms through n = 969 (2026-11-01)
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
    if (n == 0)
        return 0;  /* safety for calls below the offset */

    /* cast index to seq_t for integer arithmetic */
    seq_t nseq  = (seq_t)n;
    seq_t n1seq = (seq_t)(n - 1);

    /* floor(sqrt(2)*n) = floor(sqrt(2 * n^2)) */
    seq_t n2  = nseq  * nseq;
    seq_t n12 = n1seq * n1seq;

    seq_t x = floor_sqrt(n2  * (seq_t)2);
    seq_t y = floor_sqrt(n12 * (seq_t)2);

    return (seq_t)2 + x - y;
}
