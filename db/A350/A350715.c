/*
 * A350715  2-tone chromatic number of a wheel graph with n vertices.
 * https://oeis.org/A350715
 *
 * Offset:    4    (a(4) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = ceil((5 + sqrt(8*n-7))/2) for n > 11. Small initial
 *            values (n = 4..11) are handled by a lookup table
 * Range:     u8 n <= 11, u16 n <= 2032, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 85 terms through n = 9743 (2026-11-01)
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
    /* Handle the initial offset: a(4) … a(11) */
    if (n <= 11) {
        static const seq_t tbl[8] = {8, 8, 7, 7, 8, 7, 7, 8};
        return tbl[n - 4];
    }

    /* For n > 11: a(n) = ceil((5 + sqrt(8*n-7))/2) */
    seq_t S = ((seq_t)n * 8u) - 7;          /* 8*n - 7 */
    seq_t t = floor_sqrt(S);                /* floor(sqrt(S)) */

    seq_t ans;
    if (t * t == S) {                       /* sqrt is exact */
        if (((5 + t) & 1) == 0)             /* (5+t) even */
            ans = (5 + t) >> 1;
        else                                /* (5+t) odd */
            ans = ((5 + t) + 1) >> 1;
    } else {                                /* sqrt is not an integer */
        ans = ((5 + t) >> 1) + 1;
    }
    return ans;
}
