/*
 * A329834  Beatty sequence for (11+sqrt(73))/8.
 * https://oeis.org/A329834
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = floor(n * (11 + sqrt(73))/8)
 * Range:     u8 none, u16 n <= 14, u32 n <= 60, u64 n <= 60, u128 n <= 60,
 *            big any n
 * Validated: partial, pass, 60 terms through n = 60 (2026-11-01)
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

/* Beatty sequence for (11+sqrt(73))/8.
 * a(1) = 2, a(2) = 4, a(3) = 7, ... */
seq_t a(idx_t n)
{
    /* convert index to the integer type used for the sequence */
    seq_t n_t = (seq_t)n;

    /* compute floor(n * sqrt(73)) as floor_sqrt(73 * n^2) */
    seq_t prod = (seq_t)73 * n_t * n_t;
    seq_t q = floor_sqrt(prod);

    /* numerator of the fraction: 11*n + floor(n*sqrt(73)) */
    seq_t numer = (seq_t)11 * n_t + q;

    /* final result: floor( (11*n + floor(n*sqrt(73))) / 8 ) */
    return numer / (seq_t)8;
}
