/*
 * A281795  Number of unit squares (partially) covered by a disk of radius n
 *          centered at the origin.
 * https://oeis.org/A281795
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Sum_{k=0..n-1} 4 * ceil(sqrt(n^2 - k^2))
 * Range:     u8 n <= 7, u16 n <= 115, u32 n <= 2000, u64 n <= 2000,
 *            u128 n <= 2000, big any n
 * Validated: standard, pass, 66 terms through n = 1788 (2026-11-01)
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
        return 0;

    seq_t result = 0;
    seq_t n2 = (seq_t)n * (seq_t)n;          /* n^2 in seq_t */

    for (idx_t k = 0; k < n; ++k) {
        seq_t kk = (seq_t)k;
        seq_t diff = n2 - kk * kk;          /* n^2 - k^2, always >= 1 for k < n */

        seq_t s = floor_sqrt(diff);         /* floor sqrt of the difference */
        if (s * s < diff)                   /* ceil sqrt if not a perfect square */
            ++s;

        result += (seq_t)4 * s;             /* add 4 * ceil(sqrt(...)) */
    }

    return result;
}
