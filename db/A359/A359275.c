/*
 * A359275  a(n) = greatest integer k such that (n+k)^2 <= n^3.
 * https://oeis.org/A359275
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = -n + floor(n^(3/2)) (OEIS A359275)
 * Range:     u8 n <= 3, u16 n <= 25, u32 n <= 57, u64 n <= 57, u128 n <= 57,
 *            big any n
 * Validated: partial, pass, 58 terms through n = 57 (2026-11-01)
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
    seq_t tn = n;
    /* Compute n^3 exactly in seq_t; the values used in the tests
       fit without overflow for the supported seq_t sizes. */
    seq_t n3 = tn * tn * tn;
    seq_t root = floor_sqrt(n3);
    return root - tn;
}
