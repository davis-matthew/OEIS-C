/*
 * A334415  Nearest integer to n*(2-phi), where phi is the golden ratio
 *          (A001622).
 * https://oeis.org/A334415
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = floor((3*n - floor(n*sqrt5))/2)
 * Range:     u8 n <= 3, u16 n <= 57, u32 n <= 71, u64 n <= 71, u128 n <= 71,
 *            big any n
 * Validated: partial, pass, 72 terms through n = 71 (2026-11-01)
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

/* Helper: floor(n*sqrt5) using integer arithmetic */
static inline seq_t floor_n_sqrt5(idx_t n)
{
    seq_t sq = (seq_t)n * (seq_t)n;   /* n^2 */
    sq = 5 * sq;                     /* 5*n^2 */
    return floor_sqrt(sq);           /* floor(sqrt(5*n^2)) = floor(n*sqrt5) */
}

seq_t a(idx_t n)
{
    seq_t nn = (seq_t)n;
    seq_t m  = floor_n_sqrt5(n);     /* floor(n*sqrt5) */
    seq_t val = 3 * nn - m;          /* 3*n - floor(n*sqrt5) */
    return val / 2;                  /* floor of the division gives the rounded value */
}
