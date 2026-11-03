/*
 * A372231  Fixed points of A372341.
 * https://oeis.org/A372231
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n*(n+3)/2 - floor(n*phi) where phi = (1+sqrt(5))/2 Using
 *            floor(n*phi) = (n + floor(sqrt(5*n*n)))/2 Offset: 1 (a(1) = 1)
 * Range:     u8 n <= 3, u16 n <= 54, u32 n <= 54, u64 n <= 54, u128 n <= 54,
 *            big any n
 * Validated: partial, pass, 54 terms through n = 54 (2026-11-01)
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
    /* Promote n to seq_t for arithmetic. */
    seq_t nn = n;

    /* Triangular-like term: n*(n+3)/2. */
    seq_t tri = nn * (nn + 3) / 2;

    /* Compute floor(sqrt(5*n*n)).  Use seq_t to make it work for arbitrary precision. */
    seq_t sq = nn * nn;      /* n*n */
    sq *= 5;                 /* 5*n*n */
    seq_t root = floor_sqrt(sq);

    /* floor(n*phi) = (n + floor(sqrt(5*n*n))) / 2 */
    seq_t floor_phi = (nn + root) / 2;

    return tri - floor_phi;
}
