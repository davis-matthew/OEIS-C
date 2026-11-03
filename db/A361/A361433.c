/*
 * A361433  a(n) = number of squares in the n-th antidiagonal of the natural
 *          number array, A000027.
 * https://oeis.org/A361433
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 1 if the consecutive block [T(n-1)+1, T(n-1)+n] (where
 *            T(k)=k*(k+1)/2) contains a perfect square; otherwise 0
 * Range:     u8 n <= 11, u16 n <= 86, u32 n <= 86, u64 n <= 86, u128 n <= 86,
 *            big any n
 * Validated: partial, pass, 86 terms through n = 86 (2026-11-01)
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
    /* compute triangular number T(n-1) = (n-1)*n/2 in seq_t */
    seq_t base = (seq_t)n * ((seq_t)n - 1) / 2;

    /* block limits: [base+1, base+n] */
    seq_t low  = base + 1;
    seq_t high = base + (seq_t)n;

    /* floor of sqrt(low) */
    seq_t s = floor_sqrt(low);

    /* ceiling of sqrt(low) */
    seq_t k = s;
    if (k * k < low) /* means low is not a perfect square */
        k = k + 1;

    /* check if k^2 lies within the block */
    return (k * k <= high) ? 1 : 0;
}
