/*
 * A387646  Characteristic function of A000537.
 * https://oeis.org/A387646
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 1 if n = ((⌊(4*n)^(1/4)⌋*(⌊(4*n)^(1/4)⌋+1)/2)^2) else 0
 * Range:     u8 n <= 15, u16 n <= 86, u32 n <= 86, u64 n <= 86, u128 n <= 86,
 *            big any n
 * Validated: partial, pass, 87 terms through n = 86 (2026-11-01)
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
    /* compute 4*n in seq_t */
    seq_t m = (seq_t)n * (seq_t)4;

    /* first square root: floor(sqrt(4*n)) */
    seq_t s1 = floor_sqrt(m);

    /* second square root: k = floor((4*n)^(1/4)) */
    seq_t k = floor_sqrt(s1);

    /* triangular number t = k*(k+1)/2 */
    seq_t t = k * (k + (seq_t)1);
    t /= (seq_t)2;

    /* sum of cubes = t^2 */
    seq_t sum = t * t;

    return (sum == (seq_t)n) ? (seq_t)1 : (seq_t)0;
}
