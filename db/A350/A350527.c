/*
 * A350527  a(n) = c(n*r)*f(n/r), where f = floor, c = ceiling, and r = golden
 *          ratio (A001622).
 * https://oeis.org/A350527
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = ceil(n*φ) * floor(n*(φ−1)) where φ = (1+√5)/2 is the
 *            golden ratio
 * Range:     u8 n <= 3, u16 n <= 50, u32 n <= 50, u64 n <= 50, u128 n <= 50,
 *            big any n
 * Validated: partial, pass, 50 terms through n = 50 (2026-11-01)
 * Helpers:   ipow_i32
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Implementation uses only integer arithmetic:      */
/*  floor(n*φ)  = (n + floor(n*√5)) / 2                 */
/*  ceil (n*φ)  = (n + floor(n*√5)) / 2 + 1             */
/*  floor(n*(φ−1)) = (floor(n*√5) − n) / 2              */
/*  ceil (n*(φ−1)) = (floor(n*√5) − n) / 2 + 1          */

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
        return 0;   /* sequence is defined for n≥1, but guard anyway */

    /* compute floor(n*√5) as floor(√(5*n*n)) */
    seq_t t = floor_sqrt((seq_t)5 * (seq_t)n * (seq_t)n);

    /* ceil(n*φ)  = (n + t) / 2 + 1 */
    seq_t ceil_phi  = ((seq_t)n + t) / 2 + 1;

    /* floor(n*(φ−1)) = (t − n) / 2 */
    seq_t floor_phi_minus = (t - (seq_t)n) / 2;

    return ceil_phi * floor_phi_minus;
}
